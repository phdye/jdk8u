// Level 11: ClassLoading - Reflection
import java.lang.reflect.*;

public class Test_Reflection {
    private int privateField = 42;
    
    public static void main(String[] args) throws Exception {
        testFieldAccess();
        testMethodInvocation();
        testConstructor();
        testModifiers();
        testArrayReflection();
        System.out.println("[PASS] Test_Reflection");
    }
    
    static void testFieldAccess() throws Exception {
        Test_Reflection obj = new Test_Reflection();
        
        Field field = Test_Reflection.class.getDeclaredField("privateField");
        field.setAccessible(true);
        
        int value = field.getInt(obj);
        assert value == 42 : "read private field";
        
        field.setInt(obj, 100);
        assert obj.privateField == 100 : "write private field";
    }
    
    static void testMethodInvocation() throws Exception {
        Method method = String.class.getMethod("length");
        String s = "hello";
        int len = (Integer) method.invoke(s);
        assert len == 5 : "invoke method";
        
        Method staticMethod = Math.class.getMethod("abs", int.class);
        int result = (Integer) staticMethod.invoke(null, -42);
        assert result == 42 : "invoke static method";
    }
    
    static void testConstructor() throws Exception {
        Constructor<String> ctor = String.class.getConstructor(char[].class);
        String s = ctor.newInstance(new char[]{'a', 'b', 'c'});
        assert s.equals("abc") : "constructor invocation";
    }
    
    static void testModifiers() throws Exception {
        Field field = Test_Reflection.class.getDeclaredField("privateField");
        int mods = field.getModifiers();
        assert Modifier.isPrivate(mods) : "private modifier";
        assert !Modifier.isStatic(mods) : "not static";
        
        Method main = Test_Reflection.class.getMethod("main", String[].class);
        mods = main.getModifiers();
        assert Modifier.isPublic(mods) : "public modifier";
        assert Modifier.isStatic(mods) : "static modifier";
    }
    
    static void testArrayReflection() throws Exception {
        int[] arr = (int[]) Array.newInstance(int.class, 5);
        Array.setInt(arr, 2, 42);
        assert Array.getInt(arr, 2) == 42 : "array reflection";
        assert Array.getLength(arr) == 5 : "array length";
    }
}
