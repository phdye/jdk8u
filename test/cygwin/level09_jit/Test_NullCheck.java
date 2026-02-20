// Level 9: JIT - Null Check Elimination
public class Test_NullCheck {
    public static void main(String[] args) {
        String s = "test";
        int result = 0;
        
        // After profiling shows s is never null, null checks can be eliminated
        for (int i = 0; i < 50000; i++) {
            result += s.length();  // Implicit null check
            result += s.charAt(0);  // Another null check
        }
        
        assert result == 50000 * (4 + 't');
        System.out.println("[PASS] Test_NullCheck");
    }
}
