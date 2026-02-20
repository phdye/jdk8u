// Level 8: Interpreter - Synchronization
public class Test_Synchronized {
    static int counter = 0;
    static Object lock = new Object();
    
    public static void main(String[] args) throws Exception {
        testBasicMonitor();
        testReentrantMonitor();
        testWaitNotify();
        testWaitTimeout();
        testNotifyAll();
        testSynchronizedMethod();
        System.out.println("[PASS] Test_Synchronized");
    }
    
    static void testBasicMonitor() {
        counter = 0;
        synchronized (lock) {
            counter = 1;
        }
        assert counter == 1 : "basic monitor";
    }
    
    static void testReentrantMonitor() {
        counter = 0;
        synchronized (lock) {
            counter++;
            synchronized (lock) {
                counter++;
                synchronized (lock) {
                    counter++;
                }
            }
        }
        assert counter == 3 : "reentrant monitor";
    }
    
    static void testWaitNotify() throws Exception {
        final boolean[] signaled = {false};
        
        Thread waiter = new Thread(() -> {
            synchronized (lock) {
                try {
                    lock.wait();
                    signaled[0] = true;
                } catch (InterruptedException e) {}
            }
        });
        
        waiter.start();
        Thread.sleep(50);  // Let waiter enter wait
        
        synchronized (lock) {
            lock.notify();
        }
        
        waiter.join(1000);
        assert signaled[0] : "wait/notify";
    }
    
    static void testWaitTimeout() throws Exception {
        long start = System.currentTimeMillis();
        synchronized (lock) {
            lock.wait(100);
        }
        long elapsed = System.currentTimeMillis() - start;
        assert elapsed >= 90 && elapsed < 500 : "wait timeout";
    }
    
    static void testNotifyAll() throws Exception {
        final int[] woken = {0};
        final int NUM = 3;
        Thread[] threads = new Thread[NUM];
        
        for (int i = 0; i < NUM; i++) {
            threads[i] = new Thread(() -> {
                synchronized (lock) {
                    try {
                        lock.wait();
                        synchronized (Test_Synchronized.class) {
                            woken[0]++;
                        }
                    } catch (InterruptedException e) {}
                }
            });
            threads[i].start();
        }
        
        Thread.sleep(50);
        
        synchronized (lock) {
            lock.notifyAll();
        }
        
        for (Thread t : threads) t.join(1000);
        assert woken[0] == NUM : "notifyAll woke all";
    }
    
    synchronized static void syncMethod() {
        counter++;
    }
    
    static void testSynchronizedMethod() {
        counter = 0;
        syncMethod();
        assert counter == 1 : "synchronized method";
    }
}
