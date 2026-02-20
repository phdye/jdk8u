// Level 8: Interpreter - Arithmetic Operations
public class Test_Arithmetic {
    public static void main(String[] args) {
        testIntArithmetic();
        testLongArithmetic();
        testFloatArithmetic();
        testDoubleArithmetic();
        testBitOperations();
        testDivisionByZero();
        testOverflow();
        System.out.println("[PASS] Test_Arithmetic");
    }
    
    static void testIntArithmetic() {
        int a = 100, b = 7;
        assert a + b == 107 : "int add";
        assert a - b == 93 : "int sub";
        assert a * b == 700 : "int mul";
        assert a / b == 14 : "int div";
        assert a % b == 2 : "int rem";
        assert -a == -100 : "int neg";
    }
    
    static void testLongArithmetic() {
        long a = 10_000_000_000L, b = 3;
        assert a + b == 10_000_000_003L : "long add";
        assert a - b == 9_999_999_997L : "long sub";
        assert a * b == 30_000_000_000L : "long mul";
        assert a / b == 3_333_333_333L : "long div";
        assert a % b == 1L : "long rem";
    }
    
    static void testFloatArithmetic() {
        float a = 3.14f, b = 2.0f;
        assert Math.abs(a + b - 5.14f) < 0.001f : "float add";
        assert Math.abs(a - b - 1.14f) < 0.001f : "float sub";
        assert Math.abs(a * b - 6.28f) < 0.001f : "float mul";
        assert Math.abs(a / b - 1.57f) < 0.001f : "float div";
        assert Float.isNaN(0.0f / 0.0f) : "float NaN";
        assert Float.isInfinite(1.0f / 0.0f) : "float Inf";
    }
    
    static void testDoubleArithmetic() {
        double a = 3.141592653589793, b = 2.0;
        assert Math.abs(a + b - 5.141592653589793) < 1e-10 : "double add";
        assert Math.abs(a * b - 6.283185307179586) < 1e-10 : "double mul";
        assert Double.isNaN(0.0 / 0.0) : "double NaN";
        assert Double.isInfinite(1.0 / 0.0) : "double Inf";
    }
    
    static void testBitOperations() {
        int x = 0xFF00FF00;
        assert (x & 0x0F0F0F0F) == 0x0F000F00 : "and";
        assert (x | 0x0F0F0F0F) == 0xFF0FFF0F : "or";
        assert (x ^ 0x0F0F0F0F) == 0xF00FF00F : "xor";
        assert (~x) == 0x00FF00FF : "not";
        assert (1 << 31) == Integer.MIN_VALUE : "shl";
        assert (-1 >> 1) == -1 : "sar";
        assert (-1 >>> 1) == Integer.MAX_VALUE : "shr";
    }
    
    static void testDivisionByZero() {
        boolean caught = false;
        try { int x = 1 / 0; }
        catch (ArithmeticException e) { caught = true; }
        assert caught : "int div by zero";
        
        caught = false;
        try { long x = 1L / 0L; }
        catch (ArithmeticException e) { caught = true; }
        assert caught : "long div by zero";
    }
    
    static void testOverflow() {
        assert Integer.MAX_VALUE + 1 == Integer.MIN_VALUE : "int overflow";
        assert Long.MAX_VALUE + 1 == Long.MIN_VALUE : "long overflow";
        assert Integer.MIN_VALUE / -1 == Integer.MIN_VALUE : "int min div -1";
    }
}
