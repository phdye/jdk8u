// Level 9: JIT - Branch Prediction
import java.util.Random;

public class Test_BranchPrediction {
    public static void main(String[] args) {
        testPredictableBranch();
        testUnpredictableBranch();
        System.out.println("[PASS] Test_BranchPrediction");
    }
    
    static void testPredictableBranch() {
        int[] arr = new int[10000];
        for (int i = 0; i < arr.length; i++) arr[i] = i;
        
        long sum = 0;
        for (int iter = 0; iter < 100; iter++) {
            for (int i = 0; i < arr.length; i++) {
                // Always true for all but first element
                if (arr[i] > 0) {
                    sum += arr[i];
                }
            }
        }
        
        assert sum > 0;
    }
    
    static void testUnpredictableBranch() {
        int[] arr = new int[10000];
        Random rng = new Random(42);
        for (int i = 0; i < arr.length; i++) {
            arr[i] = rng.nextInt(100);
        }
        
        long sum = 0;
        for (int iter = 0; iter < 100; iter++) {
            for (int i = 0; i < arr.length; i++) {
                // ~50% branch probability
                if (arr[i] >= 50) {
                    sum += arr[i];
                }
            }
        }
        
        assert sum > 0;
    }
}
