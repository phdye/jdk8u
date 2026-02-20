// Level 12: Integration - Cryptography
import java.security.*;
import javax.crypto.*;
import java.util.*;

public class Test_Crypto {
    public static void main(String[] args) throws Exception {
        testMessageDigest();
        testSecureRandom();
        testSymmetricEncryption();
        testKeyPair();
        System.out.println("[PASS] Test_Crypto");
    }
    
    static void testMessageDigest() throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] hash = md.digest("Hello, World!".getBytes());
        assert hash.length == 32 : "SHA-256 produces 256 bits";
        
        md = MessageDigest.getInstance("MD5");
        hash = md.digest("test".getBytes());
        assert hash.length == 16 : "MD5 produces 128 bits";
    }
    
    static void testSecureRandom() throws Exception {
        SecureRandom sr = new SecureRandom();
        byte[] random = new byte[32];
        sr.nextBytes(random);
        
        // Should have reasonable entropy (not all zeros)
        int nonZero = 0;
        for (byte b : random) if (b != 0) nonZero++;
        assert nonZero > 20 : "has entropy";
    }
    
    static void testSymmetricEncryption() throws Exception {
        KeyGenerator kg = KeyGenerator.getInstance("AES");
        kg.init(128);
        SecretKey key = kg.generateKey();
        
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, key);
        
        byte[] plaintext = "Secret message!".getBytes();
        byte[] ciphertext = cipher.doFinal(plaintext);
        
        cipher.init(Cipher.DECRYPT_MODE, key);
        byte[] decrypted = cipher.doFinal(ciphertext);
        
        assert Arrays.equals(plaintext, decrypted) : "round-trip encryption";
    }
    
    static void testKeyPair() throws Exception {
        KeyPairGenerator kpg = KeyPairGenerator.getInstance("RSA");
        kpg.initialize(2048);
        KeyPair kp = kpg.generateKeyPair();
        
        assert kp.getPublic() != null;
        assert kp.getPrivate() != null;
        
        Signature sig = Signature.getInstance("SHA256withRSA");
        sig.initSign(kp.getPrivate());
        sig.update("data to sign".getBytes());
        byte[] signature = sig.sign();
        
        sig.initVerify(kp.getPublic());
        sig.update("data to sign".getBytes());
        assert sig.verify(signature) : "signature verification";
    }
}
