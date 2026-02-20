// Level 10: GC - Finalizers
public class Test_Finalizers {
    static volatile boolean finalized = false;
    
    public static void main(String[] args) throws Exception {
        createAndAbandon();
        
        // Trigger GC and finalization
        System.gc();
        System.runFinalization();
        Thread.sleep(200);
        
        assert finalized : "finalizer was called";
        System.out.println("[PASS] Test_Finalizers");
    }
    
    static void createAndAbandon() {
        new Finalizable();
    }
    
    static class Finalizable {
        @Override
        protected void finalize() {
            finalized = true;
        }
    }
}
