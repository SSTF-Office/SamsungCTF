package com.sctf2023.catchme;

import androidx.annotation.NonNull;

import java.util.Arrays;


public class MyCipher2 implements Cipher {
    public static final int BLOCK_SIZE = 4;
    public static final int KEY_SIZE = 6;
    public static final int ROUNDS = 9;

    public static final byte[] IV = "IVIV".getBytes();
    public static final byte[] KEY = "KEYKEY".getBytes();

    @NonNull
    public byte[] encrypt(@NonNull byte[] plaintext) {
        plaintext = pad(plaintext);

        final MyGenerator random = new MyGenerator(0xD35712E, 0xB62A0193);

//        byte[] key;
        byte[] block;
        byte[] prev = random.generate(BLOCK_SIZE);

        for (int i = 0; i < plaintext.length; i += BLOCK_SIZE) {
            block = Arrays.copyOfRange(plaintext, i, i + BLOCK_SIZE);
//            key = random.gen(KEY_SIZE * ROUNDS);
//            block = xor(block, prev);

            block = xor(block, prev);
            block = E(block, random);
            prev = block;

            System.arraycopy(block, 0, plaintext, i, BLOCK_SIZE);
        }

        return plaintext;
    }

    @NonNull
    public byte[] decrypt(@NonNull byte[] ciphertext) {
        final MyGenerator random = new MyGenerator(0xD35712E, 0xB62A0193);

//        byte[] key;
        byte[] block;
        byte[] prev = random.generate(BLOCK_SIZE);

        for (int i = 0; i < ciphertext.length; i += BLOCK_SIZE) {
            block = Arrays.copyOfRange(ciphertext, i, i + BLOCK_SIZE);
//            key = random.gen(KEY_SIZE * ROUNDS);
//            block = D(block, key);

            block = D(block, random);
            block = xor(block, prev);
            prev = Arrays.copyOfRange(ciphertext, i, i + BLOCK_SIZE);

            System.arraycopy(block, 0, ciphertext, i, BLOCK_SIZE);
        }

        return unpad(ciphertext);
    }

    @NonNull
    public byte[] E(@NonNull byte[] plaintext, @NonNull byte[] key) {
        assert plaintext.length == BLOCK_SIZE;
        assert key.length == KEY_SIZE * ROUNDS;

        for (int i = 0; i < ROUNDS; i++) {
            final byte[] out = new byte[BLOCK_SIZE];

            out[0] = rol(add(xor(plaintext[0], key[i * KEY_SIZE + 0]), xor(plaintext[1], key[i * KEY_SIZE + 1])), 7);
            out[1] = ror(add(xor(plaintext[1], key[i * KEY_SIZE + 2]), xor(plaintext[2], key[i * KEY_SIZE + 3])), 5);
            out[2] = rol(add(xor(plaintext[2], key[i * KEY_SIZE + 4]), xor(plaintext[3], key[i * KEY_SIZE + 5])), 3);
            out[3] = plaintext[0];

            plaintext = out;
        }

        return plaintext;
    }

    @NonNull
    public byte[] E(@NonNull byte[] plaintext, @NonNull MyGenerator random) {
        assert plaintext.length == BLOCK_SIZE;

        for (int i = 0; i < ROUNDS; i++) {
            final byte[] out = new byte[BLOCK_SIZE];
            final byte[] key = random.generate(KEY_SIZE);

            out[0] = rol(add(xor(plaintext[0], key[0]), xor(plaintext[1], key[1])), 7);
            out[1] = ror(add(xor(plaintext[1], key[2]), xor(plaintext[2], key[3])), 5);
            out[2] = rol(add(xor(plaintext[2], key[4]), xor(plaintext[3], key[5])), 3);
            out[3] = plaintext[0];

            plaintext = out;
        }

        return plaintext;
    }

    @NonNull
    public byte[] D(@NonNull byte[] ciphertext, @NonNull MyGenerator random) {
        assert ciphertext.length == BLOCK_SIZE;

        final byte[][] keys = new byte[ROUNDS][];
        for (int i = 0; i < ROUNDS; i++) {
            keys[i] = random.generate(KEY_SIZE);
        }

        for (int i = ROUNDS - 1; i >= 0; i--) {
            final byte[] out = new byte[BLOCK_SIZE];
            final byte[] key = keys[i];

            out[0] = ciphertext[3];
            out[1] = xor(sub(ror(ciphertext[0], 7), xor(out[0], key[0])), key[1]);
            out[2] = xor(sub(rol(ciphertext[1], 5), xor(out[1], key[2])), key[3]);
            out[3] = xor(sub(ror(ciphertext[2], 3), xor(out[2], key[4])), key[5]);

            ciphertext = out;
        }

        return ciphertext;
    }

    @NonNull
    public byte[] D(@NonNull byte[] ciphertext, @NonNull byte[] key) {
        assert ciphertext.length == BLOCK_SIZE;
        assert key.length == KEY_SIZE * ROUNDS;

        for (int i = ROUNDS - 1; i >= 0; i--) {
            final byte[] out = new byte[BLOCK_SIZE];

            out[0] = ciphertext[3];
            out[1] = xor(sub(ror(ciphertext[0], 7), xor(out[0], key[i * KEY_SIZE + 0])), key[i * KEY_SIZE + 1]);
            out[2] = xor(sub(rol(ciphertext[1], 5), xor(out[1], key[i * KEY_SIZE + 2])), key[i * KEY_SIZE + 3]);
            out[3] = xor(sub(ror(ciphertext[2], 3), xor(out[2], key[i * KEY_SIZE + 4])), key[i * KEY_SIZE + 5]);

            ciphertext = out;
        }

        return ciphertext;
    }

    @NonNull
    public byte[] pad(@NonNull byte[] text) {
        final byte pad = (byte) (BLOCK_SIZE - text.length % BLOCK_SIZE);
        final byte[] out = new byte[text.length + pad];

        for (int i = 0; i < out.length; i++) {
            if (i < text.length) {
                out[i] = text[i];
            } else {
                out[i] = pad;
            }
        }

        return out;
    }

    @NonNull
    public byte[] unpad(@NonNull byte[] text) {
        assert text.length > 0;

        final int pad = text[text.length - 1];
        assert pad > 0 && pad <= BLOCK_SIZE;

        return Arrays.copyOfRange(text, 0, text.length - pad);
    }

    @NonNull
    public byte[] xor(@NonNull byte[] x, @NonNull byte[] y) {
        for (int k = 0; k < x.length; k++) {
            x[k] = (byte) (x[k] ^ y[k]);
        }

        return x;
    }

    public static byte xor(byte x, byte y) {
        return (byte) (x ^ y);
    }

    public static byte add(byte x, byte y) {
        return (byte) (x + y);
    }

    public static byte sub(byte x, byte y) {
        return (byte) (x - y);
    }

    public static byte rol(byte n, int distance) {
        return (byte) ((((n & 0xff) << distance) | ((n & 0xff) >>> (8 - distance))));
    }

    public static byte ror(byte n, int distance) {
        return (byte) ((((n & 0xff) >>> distance) | ((n & 0xff) << (8 - distance))));
    }
}

