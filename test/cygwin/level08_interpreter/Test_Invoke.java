// Level 8: Interpreter - Method Invocation
public class Test_Invoke {
    public static void main(String[] args) {
        testInvokeStatic();
        testInvokeVirtual();
        testInvokeInterface();
        testInvokeSpecial();
        testVarargs();
        testRecursion();
        System.out.println("[PASS] Test_Invoke");
    }
    
    static void testInvokeStatic() {
        assert staticMethod(10) == 20 : "invokestatic";
        assert Math.max(3, 5) == 5 : "invokestatic Math";
    }
    
    static int staticMethod(int x) { return x * 2; }
    
    static void testInvokeVirtual() {
        Parent p = new Child();
        assert p.getValue() == 42 : "invokevirtual polymorphism";
        
        String s = "hello";
        assert s.length() == 5 : "invokevirtual String";
    }
    
    static void testInvokeInterface() {
        Comparable<Integer> c = Integer.valueOf(10);
        assert c.compareTo(5) > 0 : "invokeinterface";
        
        Runnable r = new Runnable() {
            public void run() {}
        };
        r.run();  // Should not throw
    }
    
    static void testInvokeSpecial() {
        Child c = new Child();
        assert c.callSuper() == 0 : "invokespecial super";
        
        // Private method call
        assert privateHelper() == 99 : "invokespecial private";
    }
    
    private static int privateHelper() { return 99; }
    
    static void testVarargs() {
        assert sum() == 0 : "varargs empty";
        assert sum(1) == 1 : "varargs one";
        assert sum(1, 2, 3, 4, 5) == 15 : "varargs many";
    }
    
    static int sum(int... values) {
        int s = 0;
        for (int v : values) s += v;
        return s;
    }
    
    static void testRecursion() {
        assert factorial(10) == 3628800 : "recursion";
        assert fibonacci(20) == 6765 : "recursion fib";
    }
    
    static long factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }
    
    static int fibonacci(int n) {
        return n <= 1 ? n : fibonacci(n-1) + fibonacci(n-2);
    }
    
    static class Parent {
        int getValue() { return 0; }
    }
    
    static class Child extends Parent {
        int getValue() { return 42; }
        int callSuper() { return super.getValue(); }
    }
}
