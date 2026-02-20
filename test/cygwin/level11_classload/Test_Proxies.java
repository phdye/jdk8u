// Level 11: ClassLoading - Dynamic Proxies
import java.lang.reflect.*;

public class Test_Proxies {
    public static void main(String[] args) {
        testBasicProxy();
        testMultipleInterfaces();
        System.out.println("[PASS] Test_Proxies");
    }
    
    interface Calculator {
        int add(int a, int b);
        int multiply(int a, int b);
    }
    
    static void testBasicProxy() {
        InvocationHandler handler = (proxy, method, args) -> {
            if (method.getName().equals("add")) {
                return (int)args[0] + (int)args[1];
            } else if (method.getName().equals("multiply")) {
                return (int)args[0] * (int)args[1];
            }
            return null;
        };
        
        Calculator calc = (Calculator) Proxy.newProxyInstance(
            Calculator.class.getClassLoader(),
            new Class<?>[] { Calculator.class },
            handler
        );
        
        assert calc.add(3, 5) == 8 : "proxy add";
        assert calc.multiply(4, 6) == 24 : "proxy multiply";
    }
    
    interface A { String methodA(); }
    interface B { String methodB(); }
    
    static void testMultipleInterfaces() {
        InvocationHandler handler = (proxy, method, args) -> {
            return method.getName();
        };
        
        Object proxy = Proxy.newProxyInstance(
            Test_Proxies.class.getClassLoader(),
            new Class<?>[] { A.class, B.class },
            handler
        );
        
        assert ((A)proxy).methodA().equals("methodA");
        assert ((B)proxy).methodB().equals("methodB");
    }
}
