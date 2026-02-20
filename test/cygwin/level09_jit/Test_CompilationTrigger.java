// Level 9: JIT - Compilation Trigger
public class Test_CompilationTrigger {
    static int counter = 0;
    
    public static void main(String[] args) {
        // Call method enough times to trigger compilation
        for (int i = 0; i < 20000; i++) {
            hotMethod(i);
        }
        
        assert counter == 20000 : "all iterations ran";
        System.out.println("[PASS] Test_CompilationTrigger");
    }
    
    static void hotMethod(int x) {
        counter++;
        // Some computation to make it worth compiling
        int result = x * x + x / 2 - x % 3;
        if (result < 0) result = -result;
    }
}
