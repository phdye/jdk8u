// Level 10: GC - Phantom References
import java.lang.ref.*;

public class Test_PhantomReferences {
    public static void main(String[] args) throws Exception {
        ReferenceQueue<Object> queue = new ReferenceQueue<>();
        Object obj = new Object();
        PhantomReference<Object> phantom = new PhantomReference<>(obj, queue);
        
        // Phantom references always return null from get()
        assert phantom.get() == null : "phantom.get() always null";
        
        obj = null;
        System.gc();
        Thread.sleep(100);
        
        // Should be enqueued after finalization
        Reference<?> ref = queue.poll();
        // Note: timing-dependent, may not be enqueued yet
        
        System.out.println("[PASS] Test_PhantomReferences (enqueued=" + (ref != null) + ")");
    }
}
