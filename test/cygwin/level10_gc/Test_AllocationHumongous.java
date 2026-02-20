// Level 10: GC - Humongous Object Allocation
public class Test_AllocationHumongous {
    public static void main(String[] args) {
        // Allocate very large objects
        byte[][] huge = new byte[10][];
        for (int i = 0; i < huge.length; i++) {
            huge[i] = new byte[1_000_000];  // 1MB each
        }
        
        System.gc();
        
        long total = 0;
        for (byte[] arr : huge) {
            total += arr.length;
            arr[0] = 1;  // Touch it
        }
        
        assert total == 10_000_000;
        System.out.println("[PASS] Test_AllocationHumongous");
    }
}
