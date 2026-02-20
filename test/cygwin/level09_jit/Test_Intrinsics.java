// Level 9: JIT - Intrinsics
public class Test_Intrinsics {
    public static void main(String[] args) {
        testStringIntrinsics();
        testMathIntrinsics();
        testArrayIntrinsics();
        testObjectIntrinsics();
        System.out.println("[PASS] Test_Intrinsics");
    }
    
    static void testStringIntrinsics() {
        String s1 = "hello world";
        String s2 = "hello world";
        
        for (int i = 0; i < 10000; i++) {
            assert s1.equals(s2) : "String.equals";
            assert s1.indexOf('w') == 6 : "String.indexOf";
            assert s1.hashCode() == s2.hashCode() : "String.hashCode";
        }
    }
    
    static void testMathIntrinsics() {
        for (int i = 0; i < 10000; i++) {
            double x = i * 0.001;
            
            // These should use CPU instructions directly
            assert Math.abs(-x) == x;
            assert Math.sqrt(x * x) >= 0;
            assert Math.sin(0) == 0;
            assert Math.cos(0) == 1;
            assert Math.min(x, x + 1) == x;
            assert Math.max(x, x - 1) == x;
        }
    }
    
    static void testArrayIntrinsics() {
        int[] src = new int[1000];
        int[] dst = new int[1000];
        
        for (int i = 0; i < src.length; i++) src[i] = i;
        
        for (int iter = 0; iter < 10000; iter++) {
            System.arraycopy(src, 0, dst, 0, 1000);
        }
        
        assert dst[500] == 500 : "arraycopy";
    }
    
    static void testObjectIntrinsics() {
        Object o1 = new Object();
        Object o2 = new Object();
        
        for (int i = 0; i < 10000; i++) {
            assert o1.getClass() == Object.class;
            assert o1.hashCode() != 0 || o2.hashCode() != 0;  // At least one non-zero
        }
    }
}
