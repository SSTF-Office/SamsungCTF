package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public class Scrambler1 implements Verifier {
    private static final int BLOCK_SIZE = 4;
    private static final int KEY_SIZE = 6;
    private static final int ROUNDS = 9;

    private static final byte[] MAGIC = {29, -119, 47, -88, 106, 38, 117, -7, -29, -75, -43, -31, 124, -38, -72, -49, -22, -29, -106, -33, 109, -68, -128, 10, -74, -1, 70, 87, -120, 103, -117, -20, 98, 89, -88, 99, -96, 79, 9, -112};

    private int p;
    private int q;

    @Override
    public boolean verify(@NonNull byte[] plaintext) {
        p = 0x0d35712e;
        q = 0xb62a0193;

        byte[] encrypted = encrypt(plaintext);

        int result = encrypted.length ^ MAGIC.length;

        for (int i = 0; i < MAGIC.length; i++) {
            result |= (i < encrypted.length ? encrypted[i] : 0xe2663ac5) ^ MAGIC[i];
        }

        return result == 0;
    }


    /** @noinspection PointlessArithmeticExpression */
    @NonNull
    private byte[] encrypt(@NonNull byte[] plaintext) {
        byte[] key;
        byte[] ciphertext;
        byte[] prev = generate(BLOCK_SIZE);
        byte[] out;
        byte a, b, c, d, e;

        byte pad;
        pad = (byte) (plaintext.length % BLOCK_SIZE);
        pad = (byte) (BLOCK_SIZE - pad);
        ciphertext = new byte[plaintext.length + pad];

        for (int i = 0; i < ciphertext.length; i++) {
            ciphertext[i] = i < plaintext.length ? plaintext[i] : pad;
        }

        for (int i = 0; i < plaintext.length; i += BLOCK_SIZE) {
            ciphertext[i + 0] ^= prev[0];
            ciphertext[i + 1] ^= prev[1];
            ciphertext[i + 2] ^= prev[2];
            ciphertext[i + 3] ^= prev[3];

            for (int j = 0; j < ROUNDS; j++) {
                out = new byte[BLOCK_SIZE];
                key = generate(KEY_SIZE);

                a = (byte) (ciphertext[i + 0] ^ key[0]);
                b = (byte) (ciphertext[i + 1] ^ key[1]);
                c = (byte) (a + b);
                d = (byte) ((c & 0xff) << 7);
                e = (byte) ((c & 0xff) >>> 1);
                out[0] = (byte) (d | e);

                a = (byte) (ciphertext[i + 1] ^ key[2]);
                b = (byte) (ciphertext[i + 2] ^ key[3]);
                c = (byte) (a + b);
                d = (byte) ((c & 0xff) >>> 5);
                e = (byte) ((c & 0xff) << 3);
                out[1] = (byte) (d | e);

                a = (byte) (ciphertext[i + 2] ^ key[4]);
                b = (byte) (ciphertext[i + 3] ^ key[5]);
                c = (byte) (a + b);
                d = (byte) ((c & 0xff) << 3);
                e = (byte) ((c & 0xff) >>> 5);
                out[2] = (byte) (d | e);

                out[3] = ciphertext[i + 0];

                ciphertext[i + 0] = out[0];
                ciphertext[i + 1] = out[1];
                ciphertext[i + 2] = out[2];
                ciphertext[i + 3] = out[3];
            }

            prev[0] = ciphertext[i + 0];
            prev[1] = ciphertext[i + 1];
            prev[2] = ciphertext[i + 2];
            prev[3] = ciphertext[i + 3];
        }

        return ciphertext;
    }

    @NonNull
    private byte[] generate(int n) {
        final byte[] out = new byte[n];

        for (int i = 0; i < n; i += Integer.BYTES) {
            q = p + q;
            p = q - p;

            for (int j = 0; j < 4 && i + j < n; j++) {
                out[i + j] = (byte) (q >> (((Integer.BYTES - 1) - j) * Byte.SIZE));
            }
        }

        return out;
    }
}
