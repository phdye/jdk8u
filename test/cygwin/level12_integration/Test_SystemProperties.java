// Level 12: Integration - System Properties
public class Test_SystemProperties {
    public static void main(String[] args) {
        // Basic properties
        assert System.getProperty("java.version") != null : "java.version";
        assert System.getProperty("java.home") != null : "java.home";
        assert System.getProperty("os.name") != null : "os.name";
        assert System.getProperty("user.dir") != null : "user.dir";
        
        // Set and get
        System.setProperty("test.property", "test.value");
        assert System.getProperty("test.property").equals("test.value");
        
        // Environment
        assert System.getenv("PATH") != null : "PATH env";
        
        System.out.println("[PASS] Test_SystemProperties");
    }
}
