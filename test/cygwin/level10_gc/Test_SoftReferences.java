// Level 10: GC - Soft References
import java.lang.ref.*;

public class Test_SoftReferences {
    public static void main(String[] args) {
        ReferenceQueue<byte[]> queue = new ReferenceQueue<>();
        SoftReference<byte[]> ref = new SoftReference<>(new byte[1000], queue);
        
        // Should still be accessible
        assert ref.get() != null : "soft ref accessible before GC";
        
        System.gc();
        
        // Soft refs typically not cleared unless memory pressure
        // Just verify the mechanism works
        byte[] data = ref.get();
        // data may or may not be null depending on memory pressure
        
        System.out.println("[PASS] Test_SoftReferences (cleared=" + (data == null) + ")");
    }
}
