// Level 9: JIT - Loop Optimizations
public class Test_LoopOptimizations {
    public static void main(String[] args) {
        testLoopUnrolling();
        testLoopInvariantCodeMotion();
        testLoopVectorization();
        System.out.println("[PASS] Test_LoopOptimizations");
    }
    
    static void testLoopUnrolling() {
        int[] arr = new int[1000];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = i;
        }
        
        long sum = 0;
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < arr.length; j++) {
                sum += arr[j];
            }
        }
        
        assert sum == 10000L * 499500L : "loop unrolling";
    }
    
    static void testLoopInvariantCodeMotion() {
        int[] arr = new int[100];
        int constant = 42;
        
        for (int iter = 0; iter < 10000; iter++) {
            int invariant = constant * 2;  // Should be hoisted
            for (int i = 0; i < arr.length; i++) {
                arr[i] = i + invariant;
            }
        }
        
        assert arr[50] == 50 + 84;
    }
    
    static void testLoopVectorization() {
        int[] a = new int[1024];
        int[] b = new int[1024];
        int[] c = new int[1024];
        
        for (int i = 0; i < 1024; i++) {
            a[i] = i;
            b[i] = i * 2;
        }
        
        // Simple vectorizable loop
        for (int iter = 0; iter < 10000; iter++) {
            for (int i = 0; i < 1024; i++) {
                c[i] = a[i] + b[i];
            }
        }
        
        assert c[100] == 100 + 200;
    }
}
