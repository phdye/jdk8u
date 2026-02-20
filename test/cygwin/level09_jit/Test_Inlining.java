// Level 9: JIT - Inlining
public class Test_Inlining {
    public static void main(String[] args) {
        long result = 0;
        
        for (int i = 0; i < 50000; i++) {
            result += outerMethod(i);
        }
        
        assert result != 0;
        System.out.println("[PASS] Test_Inlining (result=" + result + ")");
    }
    
    // Chain of small methods that should be inlined
    static int outerMethod(int x) {
        return middleMethod(x) + 1;
    }
    
    static int middleMethod(int x) {
        return innerMethod(x) * 2;
    }
    
    static int innerMethod(int x) {
        return x + 1;
    }
}
