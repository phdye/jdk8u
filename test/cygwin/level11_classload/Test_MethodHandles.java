// Level 11: ClassLoading - Method Handles
import java.lang.invoke.*;

public class Test_MethodHandles {
    static int staticField = 100;
    int instanceField = 200;
    
    public static void main(String[] args) throws Throwable {
        testMethodInvoke();
        testFieldAccess();
        testSpecialForms();
        testSpreader();
        System.out.println("[PASS] Test_MethodHandles");
    }
    
    static int staticMethod(int x, int y) { return x + y; }
    int instanceMethod(int x) { return x * 2; }
    
    static void testMethodInvoke() throws Throwable {
        MethodHandles.Lookup lookup = MethodHandles.lookup();
        
        // Static method
        MethodHandle mh = lookup.findStatic(Test_MethodHandles.class, "staticMethod",
            MethodType.methodType(int.class, int.class, int.class));
        int result = (int) mh.invokeExact(10, 20);
        assert result == 30 : "static method handle";
        
        // Instance method
        mh = lookup.findVirtual(Test_MethodHandles.class, "instanceMethod",
            MethodType.methodType(int.class, int.class));
        Test_MethodHandles obj = new Test_MethodHandles();
        result = (int) mh.invokeExact(obj, 21);
        assert result == 42 : "instance method handle";
    }
    
    static void testFieldAccess() throws Throwable {
        MethodHandles.Lookup lookup = MethodHandles.lookup();
        
        // Static field getter
        MethodHandle getter = lookup.findStaticGetter(Test_MethodHandles.class, 
            "staticField", int.class);
        int value = (int) getter.invokeExact();
        assert value == 100 : "static field getter";
        
        // Static field setter
        MethodHandle setter = lookup.findStaticSetter(Test_MethodHandles.class,
            "staticField", int.class);
        setter.invokeExact(150);
        assert staticField == 150 : "static field setter";
    }
    
    static void testSpecialForms() throws Throwable {
        // Identity
        MethodHandle identity = MethodHandles.identity(String.class);
        String s = (String) identity.invokeExact("test");
        assert s.equals("test") : "identity";
        
        // Constant
        MethodHandle constant = MethodHandles.constant(int.class, 42);
        int c = (int) constant.invokeExact();
        assert c == 42 : "constant";
    }
    
    static void testSpreader() throws Throwable {
        MethodHandles.Lookup lookup = MethodHandles.lookup();
        MethodHandle mh = lookup.findStatic(Test_MethodHandles.class, "staticMethod",
            MethodType.methodType(int.class, int.class, int.class));
        
        MethodHandle spreader = mh.asSpreader(int[].class, 2);
        int result = (int) spreader.invoke(new int[]{5, 7});
        assert result == 12 : "spreader";
    }
}
