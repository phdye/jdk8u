// Level 8: Interpreter - Exceptions
public class Test_Exceptions {
    public static void main(String[] args) throws Exception {
        testTryCatch();
        testTryFinally();
        testTryCatchFinally();
        testNestedTry();
        testRethrow();
        testMultiCatch();
        testCheckedExceptions();
        testExceptionChaining();
        testStackTrace();
        System.out.println("[PASS] Test_Exceptions");
    }
    
    static void testTryCatch() {
        boolean caught = false;
        try {
            throw new RuntimeException("test");
        } catch (RuntimeException e) {
            caught = e.getMessage().equals("test");
        }
        assert caught : "basic try-catch";
    }
    
    static void testTryFinally() {
        int[] counter = {0};
        try {
            counter[0] = 1;
        } finally {
            counter[0] = 2;
        }
        assert counter[0] == 2 : "try-finally normal";
        
        counter[0] = 0;
        try {
            counter[0] = 1;
            throw new RuntimeException();
        } catch (RuntimeException e) {
            counter[0] = 2;
        } finally {
            counter[0] = 3;
        }
        assert counter[0] == 3 : "try-catch-finally with exception";
    }
    
    static void testTryCatchFinally() {
        StringBuilder order = new StringBuilder();
        try {
            order.append("1");
            throw new RuntimeException();
        } catch (RuntimeException e) {
            order.append("2");
        } finally {
            order.append("3");
        }
        assert order.toString().equals("123") : "execution order";
    }
    
    static void testNestedTry() {
        StringBuilder order = new StringBuilder();
        try {
            order.append("A");
            try {
                order.append("B");
                throw new RuntimeException("inner");
            } catch (RuntimeException e) {
                order.append("C");
                throw new RuntimeException("rethrow");
            } finally {
                order.append("D");
            }
        } catch (RuntimeException e) {
            order.append("E");
        } finally {
            order.append("F");
        }
        assert order.toString().equals("ABCDEF") : "nested try order";
    }
    
    static void testRethrow() {
        String message = "";
        try {
            try {
                throw new RuntimeException("original");
            } catch (RuntimeException e) {
                throw new RuntimeException("wrapped", e);
            }
        } catch (RuntimeException e) {
            message = e.getMessage();
            assert e.getCause().getMessage().equals("original");
        }
        assert message.equals("wrapped") : "rethrow";
    }
    
    static void testMultiCatch() {
        int result = -1;
        try {
            if (Math.random() < 0) throw new IllegalArgumentException();
            throw new IllegalStateException();
        } catch (IllegalArgumentException | IllegalStateException e) {
            result = 1;
        }
        assert result == 1 : "multi-catch";
    }
    
    static void testCheckedExceptions() throws Exception {
        boolean caught = false;
        try {
            throwChecked();
        } catch (Exception e) {
            caught = true;
        }
        assert caught : "checked exception";
    }
    
    static void throwChecked() throws Exception {
        throw new Exception("checked");
    }
    
    static void testExceptionChaining() {
        try {
            try {
                throw new RuntimeException("root");
            } catch (RuntimeException e) {
                RuntimeException wrapper = new RuntimeException("level1", e);
                throw new RuntimeException("level2", wrapper);
            }
        } catch (RuntimeException e) {
            assert e.getMessage().equals("level2");
            assert e.getCause().getMessage().equals("level1");
            assert e.getCause().getCause().getMessage().equals("root");
        }
    }
    
    static void testStackTrace() {
        try {
            level1();
        } catch (RuntimeException e) {
            StackTraceElement[] trace = e.getStackTrace();
            assert trace.length >= 3 : "stack depth";
            assert trace[0].getMethodName().equals("level3") : "level3 in trace";
        }
    }
    
    static void level1() { level2(); }
    static void level2() { level3(); }
    static void level3() { throw new RuntimeException("from level3"); }
}
