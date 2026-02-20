// Level 8: Interpreter - Object Creation
public class Test_NewInstance {
    public static void main(String[] args) {
        testNewObject();
        testNewArray();
        testNewMultiArray();
        testAnonymousClass();
        testInnerClass();
        testLocalClass();
        System.out.println("[PASS] Test_NewInstance");
    }
    
    static void testNewObject() {
        Object o = new Object();
        assert o != null : "new Object";
        assert o.getClass() == Object.class;
        
        String s = new String("hello");
        assert s.equals("hello");
        
        StringBuilder sb = new StringBuilder();
        sb.append("test");
        assert sb.toString().equals("test");
        
        // With constructor args
        Integer i = new Integer(42);
        assert i.intValue() == 42;
    }
    
    static void testNewArray() {
        // newarray (primitive)
        int[] ints = new int[10];
        assert ints.length == 10;
        assert ints[0] == 0;  // default value
        
        // anewarray (reference)
        String[] strings = new String[5];
        assert strings.length == 5;
        assert strings[0] == null;
        
        // Zero-length arrays
        int[] empty = new int[0];
        assert empty.length == 0;
    }
    
    static void testNewMultiArray() {
        // multianewarray
        int[][] matrix = new int[3][4];
        assert matrix.length == 3;
        assert matrix[0].length == 4;
        
        int[][][] cube = new int[2][3][4];
        assert cube.length == 2;
        assert cube[0].length == 3;
        assert cube[0][0].length == 4;
        
        // Partial initialization
        int[][] partial = new int[3][];
        assert partial.length == 3;
        assert partial[0] == null;
    }
    
    interface Callback { void call(); }
    
    static void testAnonymousClass() {
        final int[] counter = {0};
        Callback cb = new Callback() {
            public void call() { counter[0]++; }
        };
        cb.call();
        assert counter[0] == 1 : "anonymous class";
    }
    
    static void testInnerClass() {
        Outer outer = new Outer(10);
        Outer.Inner inner = outer.new Inner(5);
        assert inner.sum() == 15 : "inner class";
    }
    
    static void testLocalClass() {
        final int x = 10;
        
        class Local {
            int getValue() { return x; }
        }
        
        Local local = new Local();
        assert local.getValue() == 10 : "local class";
    }
    
    static class Outer {
        int value;
        Outer(int v) { value = v; }
        
        class Inner {
            int innerValue;
            Inner(int v) { innerValue = v; }
            int sum() { return value + innerValue; }
        }
    }
}
