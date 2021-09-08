package com.example.egg;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class Egg
{
    public static void enc(InputStream in, OutputStream out)
    {
        try {
            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            SecretKeySpec key = new SecretKeySpec("kingodemperorchungmugongalmighty".getBytes("UTF-8"), "AES");
            IvParameterSpec iv = new IvParameterSpec("superduperinjung".getBytes("UTF-8"));
            cipher.init(Cipher.ENCRYPT_MODE, key, iv);

            CipherInputStream cis = new CipherInputStream(in, cipher);
            int n = 0;
            byte[] buf = new byte[4096];

            while ((n = cis.read(buf)) >= 0) {
                out.write(buf, 0, n);
            }
            out.close();

        } catch (Exception e) {
            return;
        }
    }
}
