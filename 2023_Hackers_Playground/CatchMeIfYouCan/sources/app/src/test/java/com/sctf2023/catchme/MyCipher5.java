package com.sctf2023.catchme;

import androidx.annotation.NonNull;


public class MyCipher5 extends MyCipher4 {

    /** @noinspection PointlessArithmeticExpression, ManualArrayCopy */
    @NonNull
    @Override
    public byte[] encrypt(@NonNull byte[] plaintext) {
        final Generator random = new MyGenerator(0xD35712E, 0xB62A0193);

        byte[] key;
        byte[] ciphertext;
        byte[] prev = random.generate(BLOCK_SIZE);
        byte[] out;

        final byte pad = (byte) (BLOCK_SIZE - plaintext.length % BLOCK_SIZE);
        ciphertext = new byte[plaintext.length + pad];

        for (int i = 0; i < ciphertext.length; i++) {
            if (i < plaintext.length) {
                ciphertext[i] = plaintext[i];
            } else {
                ciphertext[i] = pad;
            }
        }

        for (int i = 0; i < plaintext.length; i += BLOCK_SIZE) {
            for (int k = 0; k < BLOCK_SIZE; k++) {
                ciphertext[i + k] ^= prev[k];
            }

            for (int j = 0; j < ROUNDS; j++) {
                out = new byte[BLOCK_SIZE];
                key = random.generate(KEY_SIZE);

                byte a, b, c;

                a = xor(ciphertext[i + 0], key[0]);
                b = xor(ciphertext[i + 1], key[1]);
                c = add(a, b);
                out[0] = rol(c, 7);

                a = xor(ciphertext[i + 1], key[2]);
                b = xor(ciphertext[i + 2], key[3]);
                c = add(a, b);
                out[1] = ror(c, 5);

                a = xor(ciphertext[i + 2], key[4]);
                b = xor(ciphertext[i + 3], key[5]);
                c = add(a, b);
                out[2] = rol(c, 3);

                out[3] = ciphertext[i + 0];

                for (int k = 0; k < BLOCK_SIZE; k++) {
                    ciphertext[i + k] = out[k];
                }
            }

            for (int k = 0; k < BLOCK_SIZE; k++) {
                prev[k] = ciphertext[i + k];
            }
        }

        return ciphertext;
    }


    public static byte xor(byte x, byte y) {
        return (byte) (x ^ y);
    }

    public static byte add(byte x, byte y) {
        return (byte) (x + y);
    }

    public static byte rol(byte n, int distance) {
        return (byte) ((((n & 0xff) << distance) | ((n & 0xff) >>> (8 - distance))));
    }

    public static byte ror(byte n, int distance) {
        return (byte) ((((n & 0xff) >>> distance) | ((n & 0xff) << (8 - distance))));
    }
}

