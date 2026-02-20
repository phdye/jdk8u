// Level 9: JIT - Deoptimization
public class Test_Deoptimization {
    static Base instance = new Derived1();
    
    public static void main(String[] args) {
        // Warm up with monomorphic call site
        for (int i = 0; i < 20000; i++) {
            callVirtual();
        }
        
        // Now change the type - causes deoptimization
        instance = new Derived2();
        
        for (int i = 0; i < 1000; i++) {
            callVirtual();
        }
        
        System.out.println("[PASS] Test_Deoptimization");
    }
    
    static int callVirtual() {
        return instance.getValue();
    }
    
    static class Base {
        int getValue() { return 0; }
    }
    
    static class Derived1 extends Base {
        int getValue() { return 1; }
    }
    
    static class Derived2 extends Base {
        int getValue() { return 2; }
    }
}
