// Level 11: ClassLoading - Custom ClassLoader
import java.io.*;

public class Test_CustomClassLoader {
    public static void main(String[] args) throws Exception {
        testDelegation();
        testIsolation();
        System.out.println("[PASS] Test_CustomClassLoader");
    }
    
    static void testDelegation() throws Exception {
        // Custom loader that delegates to parent
        ClassLoader loader = new ClassLoader(Test_CustomClassLoader.class.getClassLoader()) {
            @Override
            protected Class<?> findClass(String name) throws ClassNotFoundException {
                throw new ClassNotFoundException(name);
            }
        };
        
        // Should find String through delegation
        Class<?> stringClass = loader.loadClass("java.lang.String");
        assert stringClass == String.class : "delegation works";
    }
    
    static void testIsolation() throws Exception {
        // Two independent loaders loading same "class"
        byte[] classBytes = generateSimpleClass("TestClass1");
        
        ClassLoader loader1 = new ByteArrayClassLoader(classBytes, "TestClass1");
        ClassLoader loader2 = new ByteArrayClassLoader(classBytes, "TestClass1");
        
        Class<?> c1 = loader1.loadClass("TestClass1");
        Class<?> c2 = loader2.loadClass("TestClass1");
        
        // Same name but different Class objects
        assert c1.getName().equals(c2.getName()) : "same name";
        assert c1 != c2 : "different Class objects";
        assert c1.getClassLoader() != c2.getClassLoader() : "different loaders";
    }
    
    static byte[] generateSimpleClass(String name) {
        // Minimal valid class file for a class with just default constructor
        // class TestClass1 { }
        return new byte[] {
            (byte)0xCA, (byte)0xFE, (byte)0xBA, (byte)0xBE, // magic
            0x00, 0x00, 0x00, 0x34, // version 52.0 (Java 8)
            0x00, 0x0D, // constant pool count = 13
            // Constant pool entries
            0x07, 0x00, 0x02, // #1 Class -> #2
            0x01, 0x00, 0x0A, 'T', 'e', 's', 't', 'C', 'l', 'a', 's', 's', '1', // #2 UTF8
            0x07, 0x00, 0x04, // #3 Class -> #4
            0x01, 0x00, 0x10, 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', // #4 UTF8
            0x01, 0x00, 0x06, '<', 'i', 'n', 'i', 't', '>', // #5 UTF8 <init>
            0x01, 0x00, 0x03, '(', ')', 'V', // #6 UTF8 ()V
            0x01, 0x00, 0x04, 'C', 'o', 'd', 'e', // #7 UTF8 Code
            0x0C, 0x00, 0x05, 0x00, 0x06, // #8 NameAndType -> #5, #6
            0x0A, 0x00, 0x03, 0x00, 0x08, // #9 Methodref -> #3, #8
            0x01, 0x00, 0x0F, 'L', 'i', 'n', 'e', 'N', 'u', 'm', 'b', 'e', 'r', 'T', 'a', 'b', 'l', 'e', // #10 UTF8
            0x01, 0x00, 0x0A, 'S', 'o', 'u', 'r', 'c', 'e', 'F', 'i', 'l', 'e', // #11 UTF8
            0x01, 0x00, 0x0F, 'T', 'e', 's', 't', 'C', 'l', 'a', 's', 's', '1', '.', 'j', 'a', 'v', 'a', // #12 UTF8
            // Access flags, this class, super class
            0x00, 0x21, // ACC_PUBLIC | ACC_SUPER
            0x00, 0x01, // this_class = #1
            0x00, 0x03, // super_class = #3
            0x00, 0x00, // interfaces_count = 0
            0x00, 0x00, // fields_count = 0
            0x00, 0x01, // methods_count = 1
            // Method: <init>
            0x00, 0x01, // ACC_PUBLIC
            0x00, 0x05, // name_index = #5
            0x00, 0x06, // descriptor_index = #6
            0x00, 0x01, // attributes_count = 1
            // Code attribute
            0x00, 0x07, // attribute_name_index = #7
            0x00, 0x00, 0x00, 0x11, // attribute_length = 17
            0x00, 0x01, // max_stack = 1
            0x00, 0x01, // max_locals = 1
            0x00, 0x00, 0x00, 0x05, // code_length = 5
            0x2A, // aload_0
            (byte)0xB7, 0x00, 0x09, // invokespecial #9
            (byte)0xB1, // return
            0x00, 0x00, // exception_table_length = 0
            0x00, 0x00, // attributes_count = 0
            // Class attributes
            0x00, 0x00, // attributes_count = 0
        };
    }
    
    static class ByteArrayClassLoader extends ClassLoader {
        private byte[] classBytes;
        private String className;
        
        ByteArrayClassLoader(byte[] bytes, String name) {
            this.classBytes = bytes;
            this.className = name;
        }
        
        @Override
        protected Class<?> findClass(String name) throws ClassNotFoundException {
            if (name.equals(className)) {
                return defineClass(name, classBytes, 0, classBytes.length);
            }
            throw new ClassNotFoundException(name);
        }
    }
}
