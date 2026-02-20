// Level 10: GC - Stress Test
import java.util.*;

public class Test_GCStress {
    public static void main(String[] args) {
        Random rng = new Random(42);
        List<byte[]> retained = new ArrayList<>();
        
        for (int i = 0; i < 10000; i++) {
            // Allocate
            int size = rng.nextInt(10000) + 100;
            byte[] data = new byte[size];
            
            // Sometimes retain, sometimes discard
            if (rng.nextInt(10) < 3) {
                retained.add(data);
                // Cap retained size
                if (retained.size() > 1000) {
                    retained.remove(0);
                }
            }
            
            // Occasional explicit GC
            if (i % 1000 == 0) {
                System.gc();
            }
        }
        
        System.out.println("[PASS] Test_GCStress (retained=" + retained.size() + ")");
    }
}
