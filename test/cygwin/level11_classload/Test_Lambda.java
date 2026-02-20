// Level 11: ClassLoading - Lambda Expressions
import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class Test_Lambda {
    public static void main(String[] args) {
        testSimpleLambda();
        testClosures();
        testMethodReferences();
        testStreamAPI();
        testFunctionalInterfaces();
        System.out.println("[PASS] Test_Lambda");
    }
    
    static void testSimpleLambda() {
        Runnable r = () -> {};
        r.run();  // Should not throw
        
        Function<Integer, Integer> square = x -> x * x;
        assert square.apply(5) == 25;
        
        BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;
        assert add.apply(3, 4) == 7;
    }
    
    static void testClosures() {
        int captured = 10;
        Function<Integer, Integer> addCaptured = x -> x + captured;
        assert addCaptured.apply(5) == 15;
        
        int[] mutableCapture = {0};
        Runnable increment = () -> mutableCapture[0]++;
        increment.run();
        increment.run();
        assert mutableCapture[0] == 2;
    }
    
    static void testMethodReferences() {
        // Static method reference
        Function<String, Integer> parse = Integer::parseInt;
        assert parse.apply("42") == 42;
        
        // Instance method reference
        String s = "HELLO";
        Supplier<String> lower = s::toLowerCase;
        assert lower.get().equals("hello");
        
        // Constructor reference
        Supplier<ArrayList<String>> listMaker = ArrayList::new;
        ArrayList<String> list = listMaker.get();
        assert list.isEmpty();
    }
    
    static void testStreamAPI() {
        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);
        
        int sum = numbers.stream()
            .filter(n -> n % 2 == 1)  // odd numbers
            .mapToInt(n -> n)
            .sum();
        assert sum == 9 : "stream sum of odds";
        
        List<String> strings = numbers.stream()
            .map(n -> "num" + n)
            .collect(Collectors.toList());
        assert strings.size() == 5;
        assert strings.get(0).equals("num1");
    }
    
    static void testFunctionalInterfaces() {
        Predicate<Integer> isPositive = n -> n > 0;
        assert isPositive.test(5);
        assert !isPositive.test(-3);
        
        Consumer<int[]> incrementFirst = arr -> arr[0]++;
        int[] arr = {0};
        incrementFirst.accept(arr);
        assert arr[0] == 1;
        
        Supplier<String> greeting = () -> "Hello";
        assert greeting.get().equals("Hello");
    }
}
