// Level 10: GC - Direct ByteBuffer
import java.nio.*;

public class Test_DirectByteBuffer {
    public static void main(String[] args) {
        // Direct buffers use native memory
        ByteBuffer direct = ByteBuffer.allocateDirect(1024);
        
        // Write data
        for (int i = 0; i < 256; i++) {
            direct.put((byte)i);
        }
        
        direct.flip();
        
        // Read back
        for (int i = 0; i < 256; i++) {
            assert direct.get() == (byte)i : "direct buffer read";
        }
        
        // Allocate many to stress native memory
        ByteBuffer[] buffers = new ByteBuffer[100];
        for (int i = 0; i < buffers.length; i++) {
            buffers[i] = ByteBuffer.allocateDirect(10000);
        }
        
        System.gc();  // Should reclaim unreferenced native memory
        
        System.out.println("[PASS] Test_DirectByteBuffer");
    }
}
