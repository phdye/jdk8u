// Level 9: JIT - Escape Analysis
public class Test_EscapeAnalysis {
    public static void main(String[] args) {
        long result = 0;
        
        for (int i = 0; i < 100000; i++) {
            // Point should be scalar replaced (not allocated on heap)
            result += computeWithPoint(i, i * 2);
        }
        
        assert result != 0;
        System.out.println("[PASS] Test_EscapeAnalysis (result=" + result + ")");
    }
    
    static int computeWithPoint(int x, int y) {
        Point p = new Point(x, y);  // Should not escape
        return p.getX() + p.getY();
    }
    
    static class Point {
        private final int x, y;
        
        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }
        
        int getX() { return x; }
        int getY() { return y; }
    }
}
