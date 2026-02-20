// Level 12: Integration - Regular Expressions
import java.util.regex.*;
import java.util.*;

public class Test_Regex {
    public static void main(String[] args) {
        testBasicMatching();
        testGroups();
        testReplacement();
        testSplit();
        testFlags();
        System.out.println("[PASS] Test_Regex");
    }
    
    static void testBasicMatching() {
        assert "hello".matches("h.*o") : "basic match";
        assert !"hello".matches("h.*x") : "basic non-match";
        
        Pattern p = Pattern.compile("\\d+");
        assert p.matcher("123").matches() : "digits";
        assert !p.matcher("abc").matches() : "not digits";
    }
    
    static void testGroups() {
        Pattern p = Pattern.compile("(\\w+)@(\\w+)\\.(\\w+)");
        Matcher m = p.matcher("user@example.com");
        
        assert m.matches() : "email pattern";
        assert m.group(1).equals("user") : "group 1";
        assert m.group(2).equals("example") : "group 2";
        assert m.group(3).equals("com") : "group 3";
    }
    
    static void testReplacement() {
        String result = "hello world".replaceAll("\\w+", "X");
        assert result.equals("X X") : "replaceAll";
        
        result = "aaa bbb ccc".replaceFirst("\\w+", "X");
        assert result.equals("X bbb ccc") : "replaceFirst";
    }
    
    static void testSplit() {
        String[] parts = "a,b,c".split(",");
        assert parts.length == 3;
        assert parts[1].equals("b");
        
        parts = "a::b::c".split("::");
        assert parts.length == 3;
    }
    
    static void testFlags() {
        Pattern p = Pattern.compile("HELLO", Pattern.CASE_INSENSITIVE);
        assert p.matcher("hello").matches() : "case insensitive";
        
        p = Pattern.compile("^line$", Pattern.MULTILINE);
        Matcher m = p.matcher("first\nline\nlast");
        assert m.find() : "multiline";
    }
}
