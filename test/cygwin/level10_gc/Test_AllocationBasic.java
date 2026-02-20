// Level 10: GC - Basic Allocation
public class Test_AllocationBasic {
    public static void main(String[] args) {
        // Allocate many small objects
        Object[] objects = new Object[100000];
        for (int i = 0; i < objects.length; i++) {
            objects[i] = new byte[100];
        }
        
        // Force GC
        System.gc();
        
        // Verify objects are still accessible
        int count = 0;
        for (Object o : objects) {
            if (o != null) count++;
        }
        
        assert count == 100000 : "all objects retained";
        System.out.println("[PASS] Test_AllocationBasic");
    }
}
