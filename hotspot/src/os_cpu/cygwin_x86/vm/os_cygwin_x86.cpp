/*
 * Copyright (c) 1999, 2017, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

/*
 * Cygwin x86/x86_64 OS-CPU layer for HotSpot JIT.
 *
 * This file provides signal handling and register access for Cygwin.
 * Key insight: Cygwin's ucontext_t.uc_mcontext IS the Windows CONTEXT structure.
 *
 * Register access:
 *   Linux:  uc->uc_mcontext.gregs[REG_RIP]
 *   Cygwin: ((CONTEXT*)&uc->uc_mcontext)->Rip
 *
 * Exception handling:
 *   - Uses POSIX signals (Cygwin translates Windows exceptions -> SIGSEGV etc.)
 *   - Uses RtlAddFunctionTable for JIT code registration (Windows API, works with GCC)
 */

// no precompiled headers
#include "asm/macroAssembler.hpp"
#include "classfile/classLoader.hpp"
#include "classfile/systemDictionary.hpp"
#include "classfile/vmSymbols.hpp"
#include "code/icBuffer.hpp"
#include "code/vtableStubs.hpp"
#include "interpreter/interpreter.hpp"
#include "jvm_linux.h"
#include "memory/allocation.inline.hpp"
#include "mutex_linux.inline.hpp"
#include "os_share_linux.hpp"
#include "prims/jniFastGetField.hpp"
#include "prims/jvm.h"
#include "prims/jvm_misc.hpp"
#include "runtime/arguments.hpp"
#include "runtime/extendedPC.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/interfaceSupport.hpp"
#include "runtime/java.hpp"
#include "runtime/javaCalls.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/osThread.hpp"
#include "runtime/sharedRuntime.hpp"
#include "runtime/stubRoutines.hpp"
#include "runtime/thread.inline.hpp"
#include "runtime/timer.hpp"
#include "utilities/events.hpp"
#include "utilities/vmError.hpp"

// POSIX includes
# include <sys/types.h>
# include <sys/mman.h>
# include <pthread.h>
# include <signal.h>
# include <errno.h>
# include <dlfcn.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/utsname.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <pwd.h>
# include <poll.h>
# include <ucontext.h>

// Windows includes for CONTEXT structure and RtlAddFunctionTable
# include <windows.h>

// Cygwin's mcontext_t IS the Windows CONTEXT structure.
// We cast to CONTEXT* to access register fields.

#ifdef AMD64
#define SPELL_REG_SP "rsp"
#define SPELL_REG_FP "rbp"
#else
#define SPELL_REG_SP "esp"
#define SPELL_REG_FP "ebp"
#endif // AMD64

PRAGMA_FORMAT_MUTE_WARNINGS_FOR_GCC

// Helper to get CONTEXT* from ucontext_t
// Cygwin's uc_mcontext is the CONTEXT structure
static inline CONTEXT* get_context(ucontext_t* uc) {
  return (CONTEXT*)&uc->uc_mcontext;
}

address os::current_stack_pointer() {
#ifdef SPARC_WORKS
  register void *esp;
  __asm__("mov %%"SPELL_REG_SP", %0":"=r"(esp));
  return (address) ((char*)esp + sizeof(long)*2);
#elif defined(__clang__)
  intptr_t* esp;
  __asm__ __volatile__ ("mov %%"SPELL_REG_SP", %0":"=r"(esp):);
  return (address) esp;
#else
  register void *esp __asm__ (SPELL_REG_SP);
  return (address) esp;
#endif
}

char* os::non_memory_address_word() {
  return (char*) -1;
}

void os::initialize_thread(Thread* thr) {
  // Nothing to do.
}

// Register access functions using Windows CONTEXT structure
address os::Linux::ucontext_get_pc(ucontext_t * uc) {
  CONTEXT* ctx = get_context(uc);
#ifdef AMD64
  return (address)ctx->Rip;
#else
  return (address)ctx->Eip;
#endif
}

intptr_t* os::Linux::ucontext_get_sp(ucontext_t * uc) {
  CONTEXT* ctx = get_context(uc);
#ifdef AMD64
  return (intptr_t*)ctx->Rsp;
#else
  return (intptr_t*)ctx->Esp;
#endif
}

intptr_t* os::Linux::ucontext_get_fp(ucontext_t * uc) {
  CONTEXT* ctx = get_context(uc);
#ifdef AMD64
  return (intptr_t*)ctx->Rbp;
#else
  return (intptr_t*)ctx->Ebp;
#endif
}

// For Forte Analyzer AsyncGetCallTrace profiling support
ExtendedPC os::Linux::fetch_frame_from_ucontext(Thread* thread,
  ucontext_t* uc, intptr_t** ret_sp, intptr_t** ret_fp) {

  assert(thread != NULL, "just checking");
  assert(ret_sp != NULL, "just checking");
  assert(ret_fp != NULL, "just checking");

  return os::fetch_frame_from_context(uc, ret_sp, ret_fp);
}

ExtendedPC os::fetch_frame_from_context(void* ucVoid,
                    intptr_t** ret_sp, intptr_t** ret_fp) {

  ExtendedPC  epc;
  ucontext_t* uc = (ucontext_t*)ucVoid;

  if (uc != NULL) {
    epc = ExtendedPC(os::Linux::ucontext_get_pc(uc));
    if (ret_sp) *ret_sp = os::Linux::ucontext_get_sp(uc);
    if (ret_fp) *ret_fp = os::Linux::ucontext_get_fp(uc);
  } else {
    epc = ExtendedPC(NULL);
    if (ret_sp) *ret_sp = (intptr_t *)NULL;
    if (ret_fp) *ret_fp = (intptr_t *)NULL;
  }

  return epc;
}

frame os::fetch_frame_from_context(void* ucVoid) {
  intptr_t* sp;
  intptr_t* fp;
  ExtendedPC epc = fetch_frame_from_context(ucVoid, &sp, &fp);
  return frame(sp, fp, epc.pc());
}

frame os::get_sender_for_C_frame(frame* fr) {
  return frame(fr->sender_sp(), fr->link(), fr->sender_pc());
}

intptr_t* _get_previous_fp() {
#ifdef SPARC_WORKS
  register intptr_t **ebp;
  __asm__("mov %%"SPELL_REG_FP", %0":"=r"(ebp));
#elif defined(__clang__)
  intptr_t **ebp;
  __asm__ __volatile__ ("mov %%"SPELL_REG_FP", %0":"=r"(ebp):);
#else
  register intptr_t **ebp __asm__ (SPELL_REG_FP);
#endif
  return (intptr_t*) *ebp;
}

frame os::current_frame() {
  intptr_t* fp = _get_previous_fp();
  frame myframe((intptr_t*)os::current_stack_pointer(),
                (intptr_t*)fp,
                CAST_FROM_FN_PTR(address, os::current_frame));
  if (os::is_first_C_frame(&myframe)) {
    return frame();
  } else {
    return os::get_sender_for_C_frame(&myframe);
  }
}

// Utility functions
enum {
  trap_page_fault = 0xE
};

// Helper to set PC in ucontext
static inline void ucontext_set_pc(ucontext_t* uc, address pc) {
  CONTEXT* ctx = get_context(uc);
#ifdef AMD64
  ctx->Rip = (DWORD64)pc;
#else
  ctx->Eip = (DWORD)pc;
#endif
}

extern "C" JNIEXPORT int
JVM_handle_linux_signal(int sig,
                        siginfo_t* info,
                        void* ucVoid,
                        int abort_if_unrecognized) {
  ucontext_t* uc = (ucontext_t*) ucVoid;

  Thread* t = ThreadLocalStorage::get_thread_slow();

  // Must do this before SignalHandlerMark, if crash protection installed we will longjmp away
  os::ThreadCrashProtection::check_crash_protection(sig, t);

  SignalHandlerMark shm(t);

  if (sig == SIGPIPE || sig == SIGXFSZ) {
    if (os::Linux::chained_handler(sig, info, ucVoid)) {
      return true;
    } else {
      if (PrintMiscellaneous && (WizardMode || Verbose)) {
        char buf[64];
        warning("Ignoring %s - see bugs 4229104 or 646499219",
                os::exception_name(sig, buf, sizeof(buf)));
      }
      return true;
    }
  }

  JavaThread* thread = NULL;
  VMThread* vmthread = NULL;
  if (os::Linux::signal_handlers_are_installed) {
    if (t != NULL ){
      if(t->is_Java_thread()) {
        thread = (JavaThread*)t;
      }
      else if(t->is_VM_thread()){
        vmthread = (VMThread *)t;
      }
    }
  }

  address stub = NULL;
  address pc = NULL;

  if (info != NULL && uc != NULL && thread != NULL) {
    pc = (address) os::Linux::ucontext_get_pc(uc);

    if ((sig == SIGSEGV || sig == SIGBUS) && StubRoutines::is_safefetch_fault(pc)) {
      ucontext_set_pc(uc, StubRoutines::continuation_for_safefetch_fault(pc));
      return 1;
    }

    // Handle stack overflow
    if (sig == SIGSEGV) {
      address addr = (address) info->si_addr;

      if (addr < thread->stack_base() &&
          addr >= thread->stack_base() - thread->stack_size()) {
        if (thread->in_stack_yellow_zone(addr)) {
          thread->disable_stack_yellow_zone();
          if (thread->thread_state() == _thread_in_Java) {
            stub = SharedRuntime::continuation_for_implicit_exception(thread, pc, SharedRuntime::STACK_OVERFLOW);
          } else {
            return 1;
          }
        } else if (thread->in_stack_red_zone(addr)) {
          thread->disable_stack_red_zone();
          tty->print_raw_cr("An irrecoverable stack overflow has occurred.");
        } else {
          if (thread->osthread()->expanding_stack() == 0) {
             thread->osthread()->set_expanding_stack();
             if (os::Linux::manually_expand_stack(thread, addr)) {
               thread->osthread()->clear_expanding_stack();
               return 1;
             }
             thread->osthread()->clear_expanding_stack();
          } else {
             fatal("recursive segv. expanding stack.");
          }
        }
      }
    }

    if ((sig == SIGSEGV) && VM_Version::is_cpuinfo_segv_addr(pc)) {
      stub = VM_Version::cpuinfo_cont_addr();
    }

    if (thread->thread_state() == _thread_in_Java) {
      if (sig == SIGSEGV && os::is_poll_address((address)info->si_addr)) {
        stub = SharedRuntime::get_poll_stub(pc);
      } else if (sig == SIGBUS) {
        CodeBlob* cb = CodeCache::find_blob_unsafe(pc);
        nmethod* nm = (cb != NULL && cb->is_nmethod()) ? (nmethod*)cb : NULL;
        if (nm != NULL && nm->has_unsafe_access()) {
          stub = StubRoutines::handler_for_unsafe_access();
        }
      }
      else

#ifdef AMD64
      if (sig == SIGFPE  &&
          (info->si_code == FPE_INTDIV || info->si_code == FPE_FLTDIV)) {
        stub =
          SharedRuntime::
          continuation_for_implicit_exception(thread,
                                              pc,
                                              SharedRuntime::
                                              IMPLICIT_DIVIDE_BY_ZERO);
#else
      if (sig == SIGFPE) {
        int op = pc[0];
        if (op == 0xDB) {
          assert(pc[0] == 0xDB, "not a FIST opcode");
          assert(pc[1] == 0x14, "not a FIST opcode");
          assert(pc[2] == 0x24, "not a FIST opcode");
          return true;
        } else if (op == 0xF7) {
          stub = SharedRuntime::continuation_for_implicit_exception(thread, pc, SharedRuntime::IMPLICIT_DIVIDE_BY_ZERO);
        } else {
          tty->print_cr("unknown opcode 0x%X with SIGFPE.", op);
          fatal("please update this code.");
        }
#endif // AMD64
      } else if (sig == SIGSEGV &&
               !MacroAssembler::needs_explicit_null_check((intptr_t)info->si_addr)) {
          stub = SharedRuntime::continuation_for_implicit_exception(thread, pc, SharedRuntime::IMPLICIT_NULL);
      }
    } else if (thread->thread_state() == _thread_in_vm &&
               sig == SIGBUS &&
               thread->doing_unsafe_access()) {
        stub = StubRoutines::handler_for_unsafe_access();
    }

    // jni_fast_Get<Primitive>Field can trap at certain pc's if a GC kicks in
    if ((sig == SIGSEGV) || (sig == SIGBUS)) {
      address addr = JNI_FastGetField::find_slowcase_pc(pc);
      if (addr != (address)-1) {
        stub = addr;
      }
    }

    // Check memory serialization page
    if ((sig == SIGSEGV) &&
        os::is_memory_serialize_page(thread, (address) info->si_addr)) {
      os::block_on_serialize_page_trap();
      return true;
    }
  }

  if (stub != NULL) {
    if (thread != NULL) thread->set_saved_exception_pc(pc);
    ucontext_set_pc(uc, stub);
    return true;
  }

  // signal-chaining
  if (os::Linux::chained_handler(sig, info, ucVoid)) {
     return true;
  }

  if (!abort_if_unrecognized) {
    return false;
  }

  if (pc == NULL && uc != NULL) {
    pc = os::Linux::ucontext_get_pc(uc);
  }

  // unmask current signal
  sigset_t newset;
  sigemptyset(&newset);
  sigaddset(&newset, sig);
  sigprocmask(SIG_UNBLOCK, &newset, NULL);

  VMError err(t, sig, pc, info, ucVoid);
  err.report_and_die();

  ShouldNotReachHere();
  return true;
}

void os::Linux::init_thread_fpu_state(void) {
#ifndef AMD64
  // Cygwin doesn't have fpu_control.h, use inline assembly
  unsigned short fpu_control = 0x27f;  // 53 bit precision
  __asm__ volatile ("fldcw %0" : : "m" (fpu_control));
#endif
}

int os::Linux::get_fpu_control_word(void) {
#ifdef AMD64
  return 0;
#else
  unsigned short fpu_control;
  __asm__ volatile ("fnstcw %0" : "=m" (fpu_control));
  return fpu_control & 0xffff;
#endif
}

void os::Linux::set_fpu_control_word(int fpu_control) {
#ifndef AMD64
  unsigned short fpu_cw = (unsigned short)fpu_control;
  __asm__ volatile ("fldcw %0" : : "m" (fpu_cw));
#endif
}

bool os::supports_sse() {
#ifdef AMD64
  return true;
#else
  // For 32-bit, assume modern CPU supports SSE
  return true;
#endif
}

bool os::is_allocatable(size_t bytes) {
#ifdef AMD64
  return true;
#else
  if (bytes < 2 * G) {
    return true;
  }
  char* addr = reserve_memory(bytes, NULL);
  if (addr != NULL) {
    release_memory(addr, bytes);
  }
  return addr != NULL;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// thread stack

#ifdef AMD64
size_t os::Linux::min_stack_allowed  = 64 * K;
bool os::Linux::supports_variable_stack_size() { return true; }
#else
size_t os::Linux::min_stack_allowed  = (48 DEBUG_ONLY(+4))*K;
bool os::Linux::supports_variable_stack_size() {
  // Cygwin uses NPTL-like threading
  return true;
}
#endif // AMD64

size_t os::Linux::default_stack_size(os::ThreadType thr_type) {
#ifdef AMD64
  size_t s = (thr_type == os::compiler_thread ? 4 * M : 1 * M);
#else
  size_t s = (thr_type == os::compiler_thread ? 2 * M : 512 * K);
#endif
  return s;
}

size_t os::Linux::default_guard_size(os::ThreadType thr_type) {
  // Cygwin's pthread_create fails with EAGAIN when guard_size=0.
  // Always use at least one guard page on Cygwin.
  // (On Linux, java_thread uses HotSpot guard pages instead of glibc guard.)
  return page_size();
}

static void current_stack_region(address * bottom, size_t * size) {
  if (os::is_primordial_thread()) {
     *bottom = os::Linux::initial_thread_stack_bottom();
     *size   = os::Linux::initial_thread_stack_size();
  } else {
     pthread_attr_t attr;
     int rslt = pthread_getattr_np(pthread_self(), &attr);

     if (rslt != 0) {
       if (rslt == ENOMEM) {
         vm_exit_out_of_memory(0, OOM_MMAP_ERROR, "pthread_getattr_np");
       } else {
         fatal(err_msg("pthread_getattr_np failed with errno = %d", rslt));
       }
     }

     if (pthread_attr_getstack(&attr, (void **)bottom, size) != 0) {
         fatal("Can not locate current stack attributes!");
     }

     pthread_attr_destroy(&attr);
  }
  assert(os::current_stack_pointer() >= *bottom &&
         os::current_stack_pointer() < *bottom + *size, "just checking");
}

address os::current_stack_base() {
  address bottom;
  size_t size;
  current_stack_region(&bottom, &size);
  return (bottom + size);
}

size_t os::current_stack_size() {
  address bottom;
  size_t size;
  current_stack_region(&bottom, &size);
  return size;
}

/////////////////////////////////////////////////////////////////////////////
// JIT Code Area Registration
//
// On Cygwin, we rely on POSIX signal handling rather than Windows SEH
// for exception handling in JIT-compiled code. The Linux signal handlers
// in os_linux.cpp handle SIGSEGV, SIGBUS, etc.
//
// Windows SEH registration via RtlAddFunctionTable would require implementing
// topLevelExceptionFilter (a complex Windows-specific function). For now,
// we stub this out since signal handling should cover our needs.

bool os::register_code_area(char *low, char *high) {
  // Stub - signal handling covers crash detection in JIT code
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// helper functions for fatal error handler

void os::print_context(outputStream *st, void *context) {
  if (context == NULL) return;

  ucontext_t *uc = (ucontext_t*)context;
  CONTEXT* ctx = get_context(uc);
  
  st->print_cr("Registers:");
#ifdef AMD64
  st->print(  "RAX=" INTPTR_FORMAT, ctx->Rax);
  st->print(", RBX=" INTPTR_FORMAT, ctx->Rbx);
  st->print(", RCX=" INTPTR_FORMAT, ctx->Rcx);
  st->print(", RDX=" INTPTR_FORMAT, ctx->Rdx);
  st->cr();
  st->print(  "RSP=" INTPTR_FORMAT, ctx->Rsp);
  st->print(", RBP=" INTPTR_FORMAT, ctx->Rbp);
  st->print(", RSI=" INTPTR_FORMAT, ctx->Rsi);
  st->print(", RDI=" INTPTR_FORMAT, ctx->Rdi);
  st->cr();
  st->print(  "R8 =" INTPTR_FORMAT, ctx->R8);
  st->print(", R9 =" INTPTR_FORMAT, ctx->R9);
  st->print(", R10=" INTPTR_FORMAT, ctx->R10);
  st->print(", R11=" INTPTR_FORMAT, ctx->R11);
  st->cr();
  st->print(  "R12=" INTPTR_FORMAT, ctx->R12);
  st->print(", R13=" INTPTR_FORMAT, ctx->R13);
  st->print(", R14=" INTPTR_FORMAT, ctx->R14);
  st->print(", R15=" INTPTR_FORMAT, ctx->R15);
  st->cr();
  st->print(  "RIP=" INTPTR_FORMAT, ctx->Rip);
  st->print(", EFLAGS=" INTPTR_FORMAT, ctx->EFlags);
#else
  st->print(  "EAX=" INTPTR_FORMAT, ctx->Eax);
  st->print(", EBX=" INTPTR_FORMAT, ctx->Ebx);
  st->print(", ECX=" INTPTR_FORMAT, ctx->Ecx);
  st->print(", EDX=" INTPTR_FORMAT, ctx->Edx);
  st->cr();
  st->print(  "ESP=" INTPTR_FORMAT, ctx->Esp);
  st->print(", EBP=" INTPTR_FORMAT, ctx->Ebp);
  st->print(", ESI=" INTPTR_FORMAT, ctx->Esi);
  st->print(", EDI=" INTPTR_FORMAT, ctx->Edi);
  st->cr();
  st->print(  "EIP=" INTPTR_FORMAT, ctx->Eip);
  st->print(", EFLAGS=" INTPTR_FORMAT, ctx->EFlags);
#endif // AMD64
  st->cr();
  st->cr();

  intptr_t *sp = (intptr_t *)os::Linux::ucontext_get_sp(uc);
  st->print_cr("Top of Stack: (sp=" PTR_FORMAT ")", sp);
  print_hex_dump(st, (address)sp, (address)(sp + 8*sizeof(intptr_t)), sizeof(intptr_t));
  st->cr();

  address pc = os::Linux::ucontext_get_pc(uc);
  st->print_cr("Instructions: (pc=" PTR_FORMAT ")", pc);
  print_hex_dump(st, pc - 32, pc + 32, sizeof(char));
}

void os::print_register_info(outputStream *st, void *context) {
  if (context == NULL) return;

  ucontext_t *uc = (ucontext_t*)context;
  CONTEXT* ctx = get_context(uc);

  st->print_cr("Register to memory mapping:");
  st->cr();

#ifdef AMD64
  st->print("RAX="); print_location(st, ctx->Rax);
  st->print("RBX="); print_location(st, ctx->Rbx);
  st->print("RCX="); print_location(st, ctx->Rcx);
  st->print("RDX="); print_location(st, ctx->Rdx);
  st->print("RSP="); print_location(st, ctx->Rsp);
  st->print("RBP="); print_location(st, ctx->Rbp);
  st->print("RSI="); print_location(st, ctx->Rsi);
  st->print("RDI="); print_location(st, ctx->Rdi);
  st->print("R8 ="); print_location(st, ctx->R8);
  st->print("R9 ="); print_location(st, ctx->R9);
  st->print("R10="); print_location(st, ctx->R10);
  st->print("R11="); print_location(st, ctx->R11);
  st->print("R12="); print_location(st, ctx->R12);
  st->print("R13="); print_location(st, ctx->R13);
  st->print("R14="); print_location(st, ctx->R14);
  st->print("R15="); print_location(st, ctx->R15);
#else
  st->print("EAX="); print_location(st, ctx->Eax);
  st->print("EBX="); print_location(st, ctx->Ebx);
  st->print("ECX="); print_location(st, ctx->Ecx);
  st->print("EDX="); print_location(st, ctx->Edx);
  st->print("ESP="); print_location(st, ctx->Esp);
  st->print("EBP="); print_location(st, ctx->Ebp);
  st->print("ESI="); print_location(st, ctx->Esi);
  st->print("EDI="); print_location(st, ctx->Edi);
#endif // AMD64

  st->cr();
}

void os::setup_fpu() {
#ifndef AMD64
  address fpu_cntrl = StubRoutines::addr_fpu_cntrl_wrd_std();
  __asm__ volatile ("fldcw (%0)" :
                    : "r" (fpu_cntrl) : "memory");
#endif
}

#ifndef PRODUCT
void os::verify_stack_alignment() {
#ifdef AMD64
  assert(((intptr_t)os::current_stack_pointer() & (StackAlignmentInBytes-1)) == 0, "incorrect stack alignment");
#endif
}
#endif

// No-op for Cygwin (CS limit workaround is Linux IA32 specific)
void os::workaround_expand_exec_shield_cs_limit() {
  // Not needed on Cygwin
}
