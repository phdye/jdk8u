// Level 12: Integration - Threading
import java.util.concurrent.*;
import java.util.concurrent.atomic.*;

public class Test_Threads {
    public static void main(String[] args) throws Exception {
        testBasicThread();
        testThreadPool();
        testFuture();
        testCountDownLatch();
        testCyclicBarrier();
        System.out.println("[PASS] Test_Threads");
    }
    
    static void testBasicThread() throws Exception {
        AtomicBoolean ran = new AtomicBoolean(false);
        Thread t = new Thread(() -> ran.set(true));
        t.start();
        t.join();
        assert ran.get() : "thread ran";
    }
    
    static void testThreadPool() throws Exception {
        ExecutorService pool = Executors.newFixedThreadPool(4);
        AtomicInteger counter = new AtomicInteger(0);
        
        for (int i = 0; i < 100; i++) {
            pool.submit(() -> counter.incrementAndGet());
        }
        
        pool.shutdown();
        pool.awaitTermination(5, TimeUnit.SECONDS);
        
        assert counter.get() == 100 : "all tasks completed";
    }
    
    static void testFuture() throws Exception {
        ExecutorService pool = Executors.newSingleThreadExecutor();
        Future<Integer> future = pool.submit(() -> {
            Thread.sleep(50);
            return 42;
        });
        
        assert !future.isDone() : "not done immediately";
        Integer result = future.get();
        assert result == 42 : "future result";
        assert future.isDone() : "done after get";
        
        pool.shutdown();
    }
    
    static void testCountDownLatch() throws Exception {
        int N = 5;
        CountDownLatch latch = new CountDownLatch(N);
        AtomicInteger started = new AtomicInteger(0);
        
        for (int i = 0; i < N; i++) {
            new Thread(() -> {
                started.incrementAndGet();
                latch.countDown();
            }).start();
        }
        
        latch.await();
        assert started.get() == N : "all threads started";
    }
    
    static void testCyclicBarrier() throws Exception {
        int N = 3;
        CyclicBarrier barrier = new CyclicBarrier(N);
        AtomicInteger phase1 = new AtomicInteger(0);
        AtomicInteger phase2 = new AtomicInteger(0);
        
        for (int i = 0; i < N; i++) {
            new Thread(() -> {
                try {
                    phase1.incrementAndGet();
                    barrier.await();
                    phase2.incrementAndGet();
                    barrier.await();
                } catch (Exception e) {}
            }).start();
        }
        
        Thread.sleep(200);
        assert phase1.get() == N : "phase 1 complete";
        assert phase2.get() == N : "phase 2 complete";
    }
}
