// Level 11: ClassLoading - Generics
import java.lang.reflect.*;
import java.util.*;

public class Test_Generics {
    public static void main(String[] args) throws Exception {
        testGenericClass();
        testGenericMethod();
        testTypeErasure();
        testReification();
        testWildcards();
        System.out.println("[PASS] Test_Generics");
    }
    
    static class Box<T> {
        private T value;
        void set(T v) { value = v; }
        T get() { return value; }
    }
    
    static void testGenericClass() {
        Box<String> stringBox = new Box<>();
        stringBox.set("hello");
        assert stringBox.get().equals("hello");
        
        Box<Integer> intBox = new Box<>();
        intBox.set(42);
        assert intBox.get() == 42;
    }
    
    static <T extends Comparable<T>> T max(T a, T b) {
        return a.compareTo(b) > 0 ? a : b;
    }
    
    static void testGenericMethod() {
        assert max(3, 5) == 5 : "generic max int";
        assert max("apple", "banana").equals("banana") : "generic max string";
    }
    
    static void testTypeErasure() {
        List<String> strings = new ArrayList<>();
        List<Integer> integers = new ArrayList<>();
        
        // Same class at runtime due to type erasure
        assert strings.getClass() == integers.getClass() : "type erasure";
    }
    
    static void testReification() throws Exception {
        // Can get generic info from fields
        class Container {
            List<String> strings;
            Map<Integer, List<String>> complex;
        }
        
        Field f = Container.class.getDeclaredField("strings");
        Type t = f.getGenericType();
        assert t instanceof ParameterizedType : "parameterized type";
        
        ParameterizedType pt = (ParameterizedType) t;
        assert pt.getRawType() == List.class : "raw type is List";
        assert pt.getActualTypeArguments()[0] == String.class : "type arg is String";
    }
    
    static void testWildcards() {
        List<Integer> ints = Arrays.asList(1, 2, 3);
        
        // Upper bounded wildcard
        List<? extends Number> nums = ints;
        Number n = nums.get(0);
        assert n.intValue() == 1;
        
        // Lower bounded wildcard
        List<? super Integer> supers = new ArrayList<Number>();
        supers.add(42);
    }
}
