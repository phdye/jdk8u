// Level 10: GC - Large Object Allocation
public class Test_AllocationLarge {
    public static void main(String[] args) {
        // Allocate large objects (>TLAB, maybe >region)
        byte[][] largeObjects = new byte[100][];
        for (int i = 0; i < largeObjects.length; i++) {
            largeObjects[i] = new byte[100_000];  // 100KB each
        }
        
        System.gc();
        
        // Verify
        long totalSize = 0;
        for (byte[] arr : largeObjects) {
            totalSize += arr.length;
        }
        
        assert totalSize == 100 * 100_000;
        System.out.println("[PASS] Test_AllocationLarge");
    }
}
