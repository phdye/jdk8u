// Level 8: Interpreter - Control Flow
public class Test_ControlFlow {
    public static void main(String[] args) {
        testIf();
        testLoops();
        testSwitch();
        testTableSwitch();
        testLookupSwitch();
        testStringSwitch();
        testTernary();
        System.out.println("[PASS] Test_ControlFlow");
    }
    
    static void testIf() {
        int x = 10;
        boolean result = false;
        
        if (x > 5) result = true;
        assert result : "if greater";
        
        if (x < 5) result = false;
        else result = true;
        assert result : "if-else";
        
        if (x < 5) result = false;
        else if (x < 15) result = true;
        else result = false;
        assert result : "if-elseif-else";
    }
    
    static void testLoops() {
        // for loop
        int sum = 0;
        for (int i = 1; i <= 10; i++) sum += i;
        assert sum == 55 : "for loop";
        
        // while loop
        sum = 0;
        int i = 1;
        while (i <= 10) { sum += i; i++; }
        assert sum == 55 : "while loop";
        
        // do-while loop
        sum = 0;
        i = 1;
        do { sum += i; i++; } while (i <= 10);
        assert sum == 55 : "do-while loop";
        
        // nested loops
        sum = 0;
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 4; k++)
                sum++;
        assert sum == 12 : "nested loops";
        
        // break
        sum = 0;
        for (i = 0; i < 100; i++) {
            if (i == 10) break;
            sum++;
        }
        assert sum == 10 : "break";
        
        // continue
        sum = 0;
        for (i = 0; i < 10; i++) {
            if (i % 2 == 0) continue;
            sum++;
        }
        assert sum == 5 : "continue";
        
        // labeled break
        sum = 0;
        outer: for (i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (j == 3) break outer;
                sum++;
            }
        }
        assert sum == 3 : "labeled break";
    }
    
    static void testSwitch() {
        int x = 2;
        String result = "";
        switch (x) {
            case 1: result = "one"; break;
            case 2: result = "two"; break;
            case 3: result = "three"; break;
            default: result = "other";
        }
        assert result.equals("two") : "switch int";
    }
    
    static void testTableSwitch() {
        // Dense cases generate tableswitch
        int x = 5;
        int result = -1;
        switch (x) {
            case 1: result = 10; break;
            case 2: result = 20; break;
            case 3: result = 30; break;
            case 4: result = 40; break;
            case 5: result = 50; break;
            case 6: result = 60; break;
            default: result = 0;
        }
        assert result == 50 : "tableswitch";
    }
    
    static void testLookupSwitch() {
        // Sparse cases generate lookupswitch
        int x = 1000;
        int result = -1;
        switch (x) {
            case 1: result = 1; break;
            case 100: result = 100; break;
            case 1000: result = 1000; break;
            case 10000: result = 10000; break;
            default: result = 0;
        }
        assert result == 1000 : "lookupswitch";
    }
    
    static void testStringSwitch() {
        String s = "hello";
        int result = -1;
        switch (s) {
            case "world": result = 1; break;
            case "hello": result = 2; break;
            case "foo": result = 3; break;
            default: result = 0;
        }
        assert result == 2 : "string switch";
    }
    
    static void testTernary() {
        int x = 10;
        String result = x > 5 ? "big" : "small";
        assert result.equals("big") : "ternary";
        
        // Nested ternary
        result = x > 15 ? "huge" : x > 5 ? "big" : "small";
        assert result.equals("big") : "nested ternary";
    }
}
