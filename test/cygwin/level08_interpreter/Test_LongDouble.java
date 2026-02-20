// Level 8: Interpreter - Long/Double Operations
public class Test_LongDouble {
    public static void main(String[] args) {
        testLongOperations();
        testDoubleOperations();
        testLongComparison();
        testDoubleComparison();
        testLongConstants();
        testDoubleConstants();
        System.out.println("[PASS] Test_LongDouble");
    }
    
    static void testLongOperations() {
        long a = 0x123456789ABCDEF0L;
        long b = 0x0FEDCBA987654321L;
        
        assert (a + b) == 0x2222222222222211L : "long add";
        assert (a - b) == 0x02468ACF13579BCFL : "long sub";
        
        a = 1_000_000L;
        b = 1_000_000L;
        assert (a * b) == 1_000_000_000_000L : "long mul";
        
        a = 1_000_000_000_000L;
        b = 1_000_000L;
        assert (a / b) == 1_000_000L : "long div";
        assert (a % b) == 0 : "long rem";
        
        // Shift operations (use int for shift amount)
        a = 1L;
        assert (a << 32) == 0x100000000L : "long shl";
        assert (0x100000000L >> 16) == 0x10000L : "long shr";
        assert (-1L >>> 32) == 0xFFFFFFFFL : "long ushr";
    }
    
    static void testDoubleOperations() {
        double a = 1.23456789012345678;
        double b = 9.87654321098765432;
        
        // Basic ops
        assert Math.abs((a + b) - 11.1111111011111) < 1e-10 : "double add";
        assert Math.abs((a * b) - 12.193263113702) < 1e-6 : "double mul";
        
        // Special values
        double posInf = Double.POSITIVE_INFINITY;
        double negInf = Double.NEGATIVE_INFINITY;
        double nan = Double.NaN;
        
        assert posInf + 1 == posInf : "inf + 1";
        assert Double.isNaN(posInf + negInf) : "inf - inf = NaN";
        assert Double.isNaN(nan) : "NaN check";
        assert nan != nan : "NaN != NaN";
    }
    
    static void testLongComparison() {
        long a = Long.MAX_VALUE;
        long b = Long.MIN_VALUE;
        long c = 0L;
        
        assert a > c : "max > 0";
        assert b < c : "min < 0";
        assert a > b : "max > min";
        assert !(a < b) : "not max < min";
        
        // Edge cases
        assert Long.MAX_VALUE == 9223372036854775807L;
        assert Long.MIN_VALUE == -9223372036854775808L;
    }
    
    static void testDoubleComparison() {
        double a = 1.0;
        double b = 2.0;
        double nan = Double.NaN;
        
        assert a < b : "1.0 < 2.0";
        assert b > a : "2.0 > 1.0";
        assert a == a : "a == a";
        
        // NaN comparisons always false
        assert !(nan < a) : "NaN < x false";
        assert !(nan > a) : "NaN > x false";
        assert !(nan == nan) : "NaN == NaN false";
        
        // Infinity
        assert Double.POSITIVE_INFINITY > Double.MAX_VALUE;
        assert Double.NEGATIVE_INFINITY < -Double.MAX_VALUE;
    }
    
    static void testLongConstants() {
        // LDC2_W for long constants
        long l1 = 0L;
        long l2 = 1L;
        long l3 = -1L;
        long l4 = 0x7FFFFFFFFFFFFFFFL;  // MAX_VALUE
        long l5 = 0x8000000000000000L;  // MIN_VALUE
        
        assert l1 == 0;
        assert l2 == 1;
        assert l3 == -1;
        assert l4 == Long.MAX_VALUE;
        assert l5 == Long.MIN_VALUE;
    }
    
    static void testDoubleConstants() {
        // LDC2_W for double constants
        double d1 = 0.0;
        double d2 = 1.0;
        double d3 = -1.0;
        double d4 = Math.PI;
        double d5 = Math.E;
        
        assert d1 == 0.0;
        assert d2 == 1.0;
        assert d4 > 3.14 && d4 < 3.15;
        assert d5 > 2.71 && d5 < 2.72;
    }
}
