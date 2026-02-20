// Level 12: Integration - Concurrent Collections
import java.util.concurrent.*;
import java.util.*;

public class Test_ConcurrentCollections {
    public static void main(String[] args) throws Exception {
        testConcurrentHashMap();
        testBlockingQueue();
        testCopyOnWriteList();
        testConcurrentSkipList();
        System.out.println("[PASS] Test_ConcurrentCollections");
    }
    
    static void testConcurrentHashMap() throws Exception {
        ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();
        
        // Concurrent updates
        Thread[] threads = new Thread[10];
        for (int i = 0; i < threads.length; i++) {
            final int idx = i;
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 100; j++) {
                    map.put("key" + idx + "_" + j, j);
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) t.join();
        assert map.size() == 1000 : "all entries added";
    }
    
    static void testBlockingQueue() throws Exception {
        BlockingQueue<Integer> queue = new LinkedBlockingQueue<>(10);
        
        // Producer
        Thread producer = new Thread(() -> {
            try {
                for (int i = 0; i < 100; i++) {
                    queue.put(i);
                }
            } catch (InterruptedException e) {}
        });
        
        // Consumer
        int[] consumed = {0};
        Thread consumer = new Thread(() -> {
            try {
                for (int i = 0; i < 100; i++) {
                    queue.take();
                    consumed[0]++;
                }
            } catch (InterruptedException e) {}
        });
        
        producer.start();
        consumer.start();
        producer.join();
        consumer.join();
        
        assert consumed[0] == 100 : "all consumed";
    }
    
    static void testCopyOnWriteList() throws Exception {
        CopyOnWriteArrayList<Integer> list = new CopyOnWriteArrayList<>();
        list.add(1);
        list.add(2);
        list.add(3);
        
        // Safe iteration during modification
        int sum = 0;
        for (Integer i : list) {
            sum += i;
            list.add(i + 10);  // Doesn't affect current iteration
        }
        
        assert sum == 6 : "iteration sum";
        assert list.size() == 6 : "additions visible after";
    }
    
    static void testConcurrentSkipList() {
        ConcurrentSkipListMap<Integer, String> map = new ConcurrentSkipListMap<>();
        map.put(3, "three");
        map.put(1, "one");
        map.put(2, "two");
        
        // Should be sorted
        List<Integer> keys = new ArrayList<>(map.keySet());
        assert keys.get(0) == 1;
        assert keys.get(1) == 2;
        assert keys.get(2) == 3;
    }
}
