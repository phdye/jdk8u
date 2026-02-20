// Level 10: GC - Weak References
import java.lang.ref.*;

public class Test_WeakReferences {
    public static void main(String[] args) throws Exception {
        ReferenceQueue<Object> queue = new ReferenceQueue<>();
        Object strong = new Object();
        WeakReference<Object> weak = new WeakReference<>(strong, queue);
        
        assert weak.get() != null : "weak ref with strong reference";
        
        strong = null;  // Remove strong reference
        System.gc();
        
        // Wait a bit for GC to process
        Thread.sleep(100);
        
        // Weak reference should be cleared
        assert weak.get() == null : "weak ref cleared after GC";
        
        // Reference should be enqueued
        Reference<?> polled = queue.poll();
        assert polled == weak : "weak ref enqueued";
        
        System.out.println("[PASS] Test_WeakReferences");
    }
}
