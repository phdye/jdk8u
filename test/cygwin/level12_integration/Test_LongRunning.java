// Level 12: Integration - Long Running / Stability
import java.util.*;
import java.util.concurrent.*;

public class Test_LongRunning {
    static final int DURATION_SECONDS = 5;  // Reduced for faster testing
    
    public static void main(String[] args) throws Exception {
        System.out.println("Running stability test for " + DURATION_SECONDS + " seconds...");
        
        long start = System.currentTimeMillis();
        long end = start + DURATION_SECONDS * 1000;
        
        ExecutorService pool = Executors.newFixedThreadPool(4);
        List<Future<?>> futures = new ArrayList<>();
        
        // Allocation stress
        futures.add(pool.submit(() -> {
            Random rng = new Random();
            while (System.currentTimeMillis() < end) {
                byte[] data = new byte[rng.nextInt(10000)];
                Arrays.fill(data, (byte) 0xFF);
            }
        }));
        
        // Computation
        futures.add(pool.submit(() -> {
            long sum = 0;
            while (System.currentTimeMillis() < end) {
                for (int i = 0; i < 1000; i++) {
                    sum += Math.sqrt(i) * Math.sin(i);
                }
            }
        }));
        
        // String operations
        futures.add(pool.submit(() -> {
            while (System.currentTimeMillis() < end) {
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < 100; i++) {
                    sb.append("test");
                }
                sb.toString().hashCode();
            }
        }));
        
        // Collection operations
        futures.add(pool.submit(() -> {
            while (System.currentTimeMillis() < end) {
                Map<Integer, String> map = new HashMap<>();
                for (int i = 0; i < 1000; i++) {
                    map.put(i, "value" + i);
                }
                map.clear();
            }
        }));
        
        // Wait for all
        for (Future<?> f : futures) {
            f.get();
        }
        
        pool.shutdown();
        
        long elapsed = System.currentTimeMillis() - start;
        System.out.println("[PASS] Test_LongRunning (elapsed=" + elapsed + "ms)");
    }
}
