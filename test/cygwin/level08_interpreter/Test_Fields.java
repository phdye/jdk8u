// Level 8: Interpreter - Field Access
public class Test_Fields {
    static int staticInt = 100;
    static long staticLong = 10_000_000_000L;
    static Object staticRef = "static";
    
    int instanceInt = 200;
    double instanceDouble = 3.14;
    String instanceRef = "instance";
    
    public static void main(String[] args) throws Exception {
        testStaticFields();
        testInstanceFields();
        testFieldInheritance();
        testVolatileFields();
        testFinalFields();
        System.out.println("[PASS] Test_Fields");
    }
    
    static void testStaticFields() {
        assert staticInt == 100 : "getstatic int";
        assert staticLong == 10_000_000_000L : "getstatic long";
        assert staticRef.equals("static") : "getstatic ref";
        
        staticInt = 200;
        assert staticInt == 200 : "putstatic int";
    }
    
    static void testInstanceFields() {
        Test_Fields obj = new Test_Fields();
        assert obj.instanceInt == 200 : "getfield int";
        assert obj.instanceDouble == 3.14 : "getfield double";
        assert obj.instanceRef.equals("instance") : "getfield ref";
        
        obj.instanceInt = 300;
        assert obj.instanceInt == 300 : "putfield int";
    }
    
    static void testFieldInheritance() {
        FieldChild child = new FieldChild();
        assert child.parentField == 1 : "inherited field";
        assert child.childField == 2 : "child field";
        
        FieldParent asParent = child;
        assert asParent.parentField == 1 : "field via parent ref";
    }
    
    static volatile int volatileField = 0;
    
    static void testVolatileFields() {
        volatileField = 42;
        assert volatileField == 42 : "volatile read/write";
    }
    
    static final int FINAL_INT = 12345;
    final String finalInstance = "final";
    
    static void testFinalFields() {
        assert FINAL_INT == 12345 : "final static";
        Test_Fields obj = new Test_Fields();
        assert obj.finalInstance.equals("final") : "final instance";
    }
    
    static class FieldParent {
        int parentField = 1;
    }
    
    static class FieldChild extends FieldParent {
        int childField = 2;
    }
}
