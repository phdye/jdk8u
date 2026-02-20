// Level 11: ClassLoading - Annotations
import java.lang.annotation.*;
import java.lang.reflect.*;

public class Test_Annotations {
    public static void main(String[] args) throws Exception {
        testClassAnnotation();
        testMethodAnnotation();
        testFieldAnnotation();
        testParameterAnnotation();
        testInheritedAnnotation();
        System.out.println("[PASS] Test_Annotations");
    }
    
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.TYPE)
    @interface ClassMarker { String value(); }
    
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.METHOD)
    @interface MethodMarker { int priority() default 0; }
    
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.FIELD)
    @interface FieldMarker {}
    
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.PARAMETER)
    @interface ParamMarker {}
    
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.TYPE)
    @Inherited
    @interface InheritedMarker {}
    
    @ClassMarker("test-class")
    static class Annotated {
        @FieldMarker
        int field;
        
        @MethodMarker(priority = 10)
        void method(@ParamMarker int param) {}
    }
    
    static void testClassAnnotation() {
        ClassMarker ann = Annotated.class.getAnnotation(ClassMarker.class);
        assert ann != null : "class annotation present";
        assert ann.value().equals("test-class") : "class annotation value";
    }
    
    static void testMethodAnnotation() throws Exception {
        Method m = Annotated.class.getDeclaredMethod("method", int.class);
        MethodMarker ann = m.getAnnotation(MethodMarker.class);
        assert ann != null : "method annotation present";
        assert ann.priority() == 10 : "method annotation value";
    }
    
    static void testFieldAnnotation() throws Exception {
        Field f = Annotated.class.getDeclaredField("field");
        FieldMarker ann = f.getAnnotation(FieldMarker.class);
        assert ann != null : "field annotation present";
    }
    
    static void testParameterAnnotation() throws Exception {
        Method m = Annotated.class.getDeclaredMethod("method", int.class);
        Annotation[][] paramAnns = m.getParameterAnnotations();
        assert paramAnns.length == 1 : "one parameter";
        assert paramAnns[0].length == 1 : "one annotation on parameter";
        assert paramAnns[0][0] instanceof ParamMarker : "correct annotation type";
    }
    
    @InheritedMarker
    static class Parent {}
    static class Child extends Parent {}
    
    static void testInheritedAnnotation() {
        assert Child.class.getAnnotation(InheritedMarker.class) != null : "inherited annotation";
    }
}
