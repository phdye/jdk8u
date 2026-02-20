# Makefile for Cygwin-native JDK bottom-up tests
# Levels 1-7: Native C++ tests (no JVM required)

HOTSPOT_SRC = ../../hotspot/src
CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -g -O0 \
           -I$(HOTSPOT_SRC)/os/linux/vm \
           -I$(HOTSPOT_SRC)/os_cpu/cygwin_x86/vm \
           -I$(HOTSPOT_SRC)/cpu/x86/vm

LDFLAGS_PTHREAD = -pthread
LDFLAGS_RT = -lrt

# Assembly object
ASM_SRC = $(HOTSPOT_SRC)/os_cpu/cygwin_x86/vm/cygwin_x86_64.s
ASM_OBJ = cygwin_x86_64.o

.PHONY: all clean test level01 level02 level03 level04 level05 level06 level07

all: level01 level02 level03 level04 level05 level06 level07

# ============================================================================
# Level 1: Cygwin Compatibility
# ============================================================================
LEVEL01_TESTS = level01_compat/test_signal_constants \
                level01_compat/test_types \
                level01_compat/test_rlimits \
                level01_compat/test_file_io \
                level01_compat/test_ids \
                level01_compat/test_stubs

level01: $(LEVEL01_TESTS)

level01_compat/test_%: level01_compat/test_%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# ============================================================================
# Level 2: Atomic Operations
# ============================================================================
LEVEL02_TESTS = level02_atomics/test_load_store \
                level02_atomics/test_cas \
                level02_atomics/test_cas_weak \
                level02_atomics/test_fetch_add \
                level02_atomics/test_fetch_bitwise \
                level02_atomics/test_memory_barriers \
                level02_atomics/test_exchange

level02: $(LEVEL02_TESTS)

level02_atomics/test_%: level02_atomics/test_%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS_PTHREAD) $< -o $@

# ============================================================================
# Level 3: Assembly Stubs
# ============================================================================
LEVEL03_TESTS = level03_assembly/test_copy_bytes \
                level03_assembly/test_copy_overlap \
                level03_assembly/test_copy_jlongs \
                level03_assembly/test_copy_jints \
                level03_assembly/test_spin_pause

level03: $(ASM_OBJ) $(LEVEL03_TESTS)

$(ASM_OBJ): $(ASM_SRC)
	$(CC) -c $< -o $@

level03_assembly/test_%: level03_assembly/test_%.cpp $(ASM_OBJ)
	$(CXX) $(CXXFLAGS) $< $(ASM_OBJ) -o $@

# ============================================================================
# Level 4: OS Abstraction
# ============================================================================
LEVEL04_TESTS = level04_os/test_pthread \
                level04_os/test_mmap \
                level04_os/test_signals \
                level04_os/test_timing \
                level04_os/test_pipes \
                level04_os/test_env

level04: $(LEVEL04_TESTS)

level04_os/test_%: level04_os/test_%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS_PTHREAD) $< -o $@

# ============================================================================
# Level 5: Threading Primitives
# ============================================================================
LEVEL05_TESTS = level05_threading/test_mutex \
                level05_threading/test_mutex_recursive \
                level05_threading/test_condvar \
                level05_threading/test_condvar_timeout \
                level05_threading/test_rwlock \
                level05_threading/test_barrier \
                level05_threading/test_tls

level05: $(LEVEL05_TESTS)

level05_threading/test_%: level05_threading/test_%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS_PTHREAD) $< -o $@

# ============================================================================
# Level 6: Memory Management
# ============================================================================
LEVEL06_TESTS = level06_memory/test_virtualspace \
                level06_memory/test_arena \
                level06_memory/test_codecache

level06: $(LEVEL06_TESTS)

level06_memory/test_%: level06_memory/test_%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# ============================================================================
# Level 7: Signal Context
# ============================================================================
LEVEL07_TESTS = level07_context/test_mcontext \
                level07_context/test_siginfo \
                level07_context/test_backtrace

level07: $(LEVEL07_TESTS)

level07_context/test_%: level07_context/test_%.cpp
	$(CXX) $(CXXFLAGS) -rdynamic $< -o $@

# ============================================================================
# Test Targets
# ============================================================================
test: test-level01 test-level02 test-level03 test-level04 test-level05 test-level06 test-level07
	@echo ""
	@echo "=========================================="
	@echo "All native tests (Levels 1-7) PASSED"
	@echo "=========================================="

test-level01: level01
	@echo ""
	@echo "=== Level 1: Cygwin Compatibility ==="
	@for t in $(LEVEL01_TESTS); do ./$$t || exit 1; done

test-level02: level02
	@echo ""
	@echo "=== Level 2: Atomic Operations ==="
	@for t in $(LEVEL02_TESTS); do ./$$t || exit 1; done

test-level03: level03
	@echo ""
	@echo "=== Level 3: Assembly Stubs ==="
	@for t in $(LEVEL03_TESTS); do ./$$t || exit 1; done

test-level04: level04
	@echo ""
	@echo "=== Level 4: OS Abstraction ==="
	@for t in $(LEVEL04_TESTS); do ./$$t || exit 1; done

test-level05: level05
	@echo ""
	@echo "=== Level 5: Threading Primitives ==="
	@for t in $(LEVEL05_TESTS); do ./$$t || exit 1; done

test-level06: level06
	@echo ""
	@echo "=== Level 6: Memory Management ==="
	@for t in $(LEVEL06_TESTS); do ./$$t || exit 1; done

test-level07: level07
	@echo ""
	@echo "=== Level 7: Signal Context ==="
	@for t in $(LEVEL07_TESTS); do ./$$t || exit 1; done

# ============================================================================
# Clean
# ============================================================================
clean:
	rm -f $(LEVEL01_TESTS) $(LEVEL02_TESTS) $(LEVEL03_TESTS) \
	      $(LEVEL04_TESTS) $(LEVEL05_TESTS) $(LEVEL06_TESTS) $(LEVEL07_TESTS)
	rm -f $(ASM_OBJ)
	rm -f level*/*.exe level*/*.o
