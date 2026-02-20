// Level 12: Integration - Network Socket
import java.net.*;
import java.io.*;

public class Test_NetworkSocket {
    public static void main(String[] args) throws Exception {
        testLoopback();
        testServerSocket();
        System.out.println("[PASS] Test_NetworkSocket");
    }
    
    static void testLoopback() throws Exception {
        InetAddress loopback = InetAddress.getLoopbackAddress();
        assert loopback.isLoopbackAddress();
        
        InetAddress localhost = InetAddress.getByName("localhost");
        assert localhost != null;
    }
    
    static void testServerSocket() throws Exception {
        final String MESSAGE = "Hello from server";
        
        // Start server
        ServerSocket server = new ServerSocket(0);  // Random port
        int port = server.getLocalPort();
        
        Thread serverThread = new Thread(() -> {
            try {
                Socket client = server.accept();
                PrintWriter out = new PrintWriter(client.getOutputStream(), true);
                out.println(MESSAGE);
                client.close();
            } catch (IOException e) {}
        });
        serverThread.start();
        
        // Connect client
        Socket client = new Socket("localhost", port);
        BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
        String received = in.readLine();
        
        assert received.equals(MESSAGE) : "received message";
        
        client.close();
        server.close();
        serverThread.join();
    }
}
