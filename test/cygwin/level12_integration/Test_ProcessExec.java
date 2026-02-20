// Level 12: Integration - Process Execution
import java.io.*;

public class Test_ProcessExec {
    public static void main(String[] args) throws Exception {
        testSimpleProcess();
        testProcessOutput();
        System.out.println("[PASS] Test_ProcessExec");
    }
    
    static void testSimpleProcess() throws Exception {
        // Use a command that works on Cygwin
        ProcessBuilder pb = new ProcessBuilder("true");
        Process p = pb.start();
        int exitCode = p.waitFor();
        assert exitCode == 0 : "true exit code";
    }
    
    static void testProcessOutput() throws Exception {
        ProcessBuilder pb = new ProcessBuilder("echo", "hello");
        pb.redirectErrorStream(true);
        Process p = pb.start();

        // Read output byte-by-byte to avoid BufferedInputStream.available()
        // which throws "Illegal seek" on Cygwin pipes
        InputStream in = p.getInputStream();
        StringBuilder sb = new StringBuilder();
        int b;
        while ((b = in.read()) != -1) {
            sb.append((char) b);
        }
        p.waitFor();

        String output = sb.toString();
        assert output.contains("hello") : "process output: " + output;
    }
}
