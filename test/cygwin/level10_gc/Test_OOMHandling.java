// Level 10: GC - OutOfMemoryError Handling
import java.util.ArrayList;
import java.util.List;

public class Test_OOMHandling {
    public static void main(String[] args) {
        boolean caughtOOM = false;
        List<byte[]> leak = new ArrayList<>();
        
        try {
            while (true) {
                leak.add(new byte[1_000_000]);  // 1MB chunks
            }
        } catch (OutOfMemoryError e) {
            caughtOOM = true;
            leak.clear();  // Free memory
            leak = null;
            System.gc();
        }
        
        assert caughtOOM : "OOM was thrown and caught";
        
        // Should be able to allocate after recovery
        byte[] afterRecovery = new byte[1000];
        assert afterRecovery != null;
        
        System.out.println("[PASS] Test_OOMHandling");
    }
}
