// Level 12: Integration - File I/O
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class Test_FileIO {
    public static void main(String[] args) throws Exception {
        testTraditionalIO();
        testNIO();
        testTempFiles();
        System.out.println("[PASS] Test_FileIO");
    }
    
    static void testTraditionalIO() throws Exception {
        File temp = File.createTempFile("test", ".txt");
        temp.deleteOnExit();
        
        // Write
        try (FileWriter fw = new FileWriter(temp);
             BufferedWriter bw = new BufferedWriter(fw)) {
            bw.write("Hello\n");
            bw.write("World\n");
        }
        
        // Read
        List<String> lines = new ArrayList<>();
        try (FileReader fr = new FileReader(temp);
             BufferedReader br = new BufferedReader(fr)) {
            String line;
            while ((line = br.readLine()) != null) {
                lines.add(line);
            }
        }
        
        assert lines.size() == 2 : "two lines";
        assert lines.get(0).equals("Hello") : "first line";
    }
    
    static void testNIO() throws Exception {
        Path temp = Files.createTempFile("test", ".txt");
        
        // Write
        Files.write(temp, Arrays.asList("Line 1", "Line 2", "Line 3"));
        
        // Read
        List<String> lines = Files.readAllLines(temp);
        assert lines.size() == 3;
        
        // Attributes
        assert Files.size(temp) > 0;
        assert Files.isRegularFile(temp);
        
        Files.delete(temp);
        assert !Files.exists(temp);
    }
    
    static void testTempFiles() throws Exception {
        Path tempDir = Files.createTempDirectory("testdir");
        assert Files.isDirectory(tempDir);
        
        Path tempFile = Files.createTempFile(tempDir, "file", ".tmp");
        assert Files.exists(tempFile);
        
        Files.delete(tempFile);
        Files.delete(tempDir);
    }
}
