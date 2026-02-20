// Level 9: JIT - Register Allocation
public class Test_RegisterAllocation {
    public static void main(String[] args) {
        // Use many local variables to stress register allocation
        long result = 0;
        
        for (int iter = 0; iter < 10000; iter++) {
            result += computeWithManyLocals(iter);
        }
        
        assert result != 0;
        System.out.println("[PASS] Test_RegisterAllocation");
    }
    
    static long computeWithManyLocals(int seed) {
        // More locals than registers
        long a = seed;
        long b = seed + 1;
        long c = seed + 2;
        long d = seed + 3;
        long e = seed + 4;
        long f = seed + 5;
        long g = seed + 6;
        long h = seed + 7;
        long i = seed + 8;
        long j = seed + 9;
        long k = seed + 10;
        long l = seed + 11;
        long m = seed + 12;
        long n = seed + 13;
        long o = seed + 14;
        long p = seed + 15;
        
        // Use all of them
        return (a + b) * (c + d) - (e + f) * (g + h) +
               (i + j) * (k + l) - (m + n) * (o + p);
    }
}
