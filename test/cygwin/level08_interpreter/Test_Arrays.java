// Level 8: Interpreter - Array Operations
public class Test_Arrays {
    public static void main(String[] args) {
        testPrimitiveArrays();
        testObjectArrays();
        testMultiDimensional();
        testArrayBounds();
        testArrayStore();
        testArrayClone();
        testArrayCopy();
        System.out.println("[PASS] Test_Arrays");
    }
    
    static void testPrimitiveArrays() {
        boolean[] bools = new boolean[3];
        bools[0] = true; bools[1] = false; bools[2] = true;
        assert bools[0] && !bools[1] && bools[2];
        
        byte[] bytes = new byte[256];
        for (int i = 0; i < 256; i++) bytes[i] = (byte)i;
        assert bytes[127] == 127 && bytes[128] == -128;
        
        char[] chars = new char[] {'H', 'e', 'l', 'l', 'o'};
        assert new String(chars).equals("Hello");
        
        short[] shorts = new short[] {Short.MIN_VALUE, 0, Short.MAX_VALUE};
        assert shorts[0] == -32768 && shorts[2] == 32767;
        
        int[] ints = new int[100];
        for (int i = 0; i < 100; i++) ints[i] = i * i;
        assert ints[10] == 100;
        
        long[] longs = new long[] {Long.MIN_VALUE, 0L, Long.MAX_VALUE};
        assert longs[2] == 9223372036854775807L;
        
        float[] floats = new float[] {1.0f, 2.0f, 3.0f};
        assert floats[0] + floats[1] + floats[2] == 6.0f;
        
        double[] doubles = new double[] {Math.PI, Math.E};
        assert doubles[0] > 3.14 && doubles[1] > 2.71;
    }
    
    static void testObjectArrays() {
        String[] strings = new String[] {"a", "b", "c"};
        assert strings.length == 3;
        assert strings[1].equals("b");
        
        Object[] objects = new Object[5];
        objects[0] = "string";
        objects[1] = 42;
        objects[2] = 3.14;
        objects[3] = new int[3];
        objects[4] = null;
        
        assert objects[0] instanceof String;
        assert objects[1] instanceof Integer;
        assert objects[4] == null;
    }
    
    static void testMultiDimensional() {
        int[][] matrix = new int[3][4];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                matrix[i][j] = i * 4 + j;
        assert matrix[2][3] == 11;
        
        int[][][] cube = new int[2][3][4];
        cube[1][2][3] = 42;
        assert cube[1][2][3] == 42;
        
        // Ragged array
        int[][] ragged = new int[3][];
        ragged[0] = new int[1];
        ragged[1] = new int[2];
        ragged[2] = new int[3];
        assert ragged[2].length == 3;
    }
    
    static void testArrayBounds() {
        int[] arr = new int[10];
        
        boolean caught = false;
        try { int x = arr[-1]; }
        catch (ArrayIndexOutOfBoundsException e) { caught = true; }
        assert caught : "negative index";
        
        caught = false;
        try { int x = arr[10]; }
        catch (ArrayIndexOutOfBoundsException e) { caught = true; }
        assert caught : "index == length";
        
        caught = false;
        try { int x = arr[100]; }
        catch (ArrayIndexOutOfBoundsException e) { caught = true; }
        assert caught : "large index";
    }
    
    static void testArrayStore() {
        Object[] objects = new String[3];
        objects[0] = "valid";
        
        boolean caught = false;
        try { objects[1] = Integer.valueOf(42); }
        catch (ArrayStoreException e) { caught = true; }
        assert caught : "ArrayStoreException";
    }
    
    static void testArrayClone() {
        int[] original = {1, 2, 3, 4, 5};
        int[] cloned = original.clone();
        
        assert cloned.length == original.length;
        assert cloned != original;
        cloned[0] = 999;
        assert original[0] == 1; // independent
    }
    
    static void testArrayCopy() {
        int[] src = {1, 2, 3, 4, 5};
        int[] dst = new int[5];
        System.arraycopy(src, 0, dst, 0, 5);
        assert dst[4] == 5;
        
        // Overlapping copy
        int[] arr = {1, 2, 3, 4, 5};
        System.arraycopy(arr, 0, arr, 1, 4);
        assert arr[0] == 1 && arr[1] == 1 && arr[4] == 4;
    }
}
