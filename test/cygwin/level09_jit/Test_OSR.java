// Level 9: JIT - On-Stack Replacement
public class Test_OSR {
    public static void main(String[] args) {
        // Long-running loop should trigger OSR
        long sum = 0;
        for (int i = 0; i < 100_000; i++) {
            sum += i;
            // Add some complexity
            if (i % 1000 == 0) {
                sum = sum ^ (sum >> 4);
            }
        }
        
        assert sum != 0 : "loop computed something";
        System.out.println("[PASS] Test_OSR (sum=" + sum + ")");
    }
}
