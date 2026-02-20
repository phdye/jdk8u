// Level 8: Interpreter - Type Conversions
public class Test_TypeConversions {
    public static void main(String[] args) {
        testWideningPrimitive();
        testNarrowingPrimitive();
        testReferenceConversions();
        testBoxingUnboxing();
        testInstanceOf();
        System.out.println("[PASS] Test_TypeConversions");
    }
    
    static void testWideningPrimitive() {
        byte b = 100;
        short s = b;
        int i = s;
        long l = i;
        float f = l;
        double d = f;
        
        assert d == 100.0 : "widening chain";
        
        char c = 'A';
        i = c;
        assert i == 65 : "char to int";
    }
    
    static void testNarrowingPrimitive() {
        double d = 123.456;
        float f = (float)d;
        long l = (long)f;
        int i = (int)l;
        short s = (short)i;
        byte b = (byte)s;
        char c = (char)i;
        
        assert b == 123 : "narrowing to byte";
        assert c == '{' : "int to char";
        
        // Truncation
        i = 0x12345678;
        s = (short)i;
        assert s == 0x5678 : "truncation to short";
        
        b = (byte)i;
        assert b == 0x78 : "truncation to byte";
        
        // Float to int truncation
        d = 3.99;
        i = (int)d;
        assert i == 3 : "double to int truncation";
        
        // Overflow
        d = 1e20;
        i = (int)d;
        assert i == Integer.MAX_VALUE : "overflow to int max";
    }
    
    static void testReferenceConversions() {
        // Upcast (implicit)
        String s = "hello";
        Object o = s;
        CharSequence cs = s;
        
        assert o instanceof String;
        assert cs instanceof String;
        
        // Downcast (explicit)
        String s2 = (String)o;
        assert s2.equals("hello");
        
        // Failed downcast
        boolean caught = false;
        try {
            o = Integer.valueOf(42);
            s2 = (String)o;
        } catch (ClassCastException e) {
            caught = true;
        }
        assert caught : "ClassCastException";
    }
    
    static void testBoxingUnboxing() {
        // Boxing
        Integer i = 42;
        Long l = 100L;
        Double d = 3.14;
        Boolean b = true;
        
        assert i.intValue() == 42;
        assert l.longValue() == 100L;
        
        // Unboxing
        int ip = i;
        long lp = l;
        double dp = d;
        boolean bp = b;
        
        assert ip == 42;
        assert bp == true;
        
        // Integer cache
        Integer i1 = 100;
        Integer i2 = 100;
        assert i1 == i2 : "Integer cache -128 to 127";
        
        Integer i3 = 200;
        Integer i4 = 200;
        assert i3 != i4 : "Integer outside cache";
        assert i3.equals(i4) : "Integer equals";
    }
    
    static void testInstanceOf() {
        Object o = "hello";
        assert o instanceof String;
        assert o instanceof CharSequence;
        assert o instanceof Object;
        assert !(o instanceof Integer);
        
        o = null;
        assert !(o instanceof Object) : "null instanceof always false";
        
        // Array instanceof
        int[] arr = new int[5];
        Object arrObj = arr;
        assert arr instanceof int[];
        assert arr instanceof Object;
        assert !(arrObj instanceof long[]);
    }
}
