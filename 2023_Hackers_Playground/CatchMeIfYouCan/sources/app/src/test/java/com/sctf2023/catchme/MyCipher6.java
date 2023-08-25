package com.sctf2023.catchme;

import androidx.annotation.NonNull;


public class MyCipher6 extends MyCipher5 {

    /** @noinspection PointlessArithmeticExpression */
    @NonNull
    @Override
    public byte[] encrypt(@NonNull byte[] plaintext) {
        final Generator random = new MyGenerator(0xD35712E, 0xB62A0193);

        byte[] key;
        byte[] ciphertext;
        byte[] prev = random.generate(BLOCK_SIZE);
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
                key = random.generate(KEY_SIZE);

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
}

