package com.sctf2023.catchme;

import androidx.annotation.NonNull;

import java.util.Arrays;

import static java.nio.charset.StandardCharsets.UTF_8;

public class MyScrambler2 {
    private static final int BLOCK_SIZE = 4;
    private static final int KEY_SIZE = 6;
    private static final int ROUNDS = 9;

    private static final byte[] MAGIC = {77, -87, 98, 90, -123, -118, 25, 41, -73, 0, 88, -71, 2, -42, 17, -49, -97, -111, -2, -94, -88, -77, 85, 32, 73, -32, 2, -124, 58, 110, 84, 121, 106, 51, 95, 63, 31, -33, -103, -56};
    private long a;
    private long b;

    public void test() {
        System.out.println(new String(decrypt(MAGIC), UTF_8));
    }

    public boolean verify(@NonNull byte[] plaintext) {
        byte[] ciphertext = encrypt(plaintext);
        int result = 0;

        result |= MAGIC[27] ^ (27 >= ciphertext.length ? 0x7a476371 : ciphertext[27]);
        result |= (ciphertext.length > 7 ? ciphertext[7] : 0x78e9a0e1) ^ MAGIC[7];
        result |= (9 >= ciphertext.length ? 0x04a8628d : ciphertext[9]) ^ MAGIC[9];
        result |= MAGIC[38] ^ (38 < ciphertext.length ? ciphertext[38] : 0xe91a88d0);
        result |= (ciphertext.length <= 33 ? 0xd3da28f8 : ciphertext[33]) ^ MAGIC[33];
        result |= (22 < ciphertext.length ? ciphertext[22] : 0x5e508e98) ^ MAGIC[22];
        result |= MAGIC[24] ^ (24 >= ciphertext.length ? 0x00d2f59f : ciphertext[24]);
        result |= (ciphertext.length > 39 ? ciphertext[39] : 0xdc794b90) ^ MAGIC[39];
        result |= (29 >= ciphertext.length ? 0x90778847 : ciphertext[29]) ^ MAGIC[29];
        result |= MAGIC[25] ^ (25 < ciphertext.length ? ciphertext[25] : 0xe293836a);
        result |= (ciphertext.length <= 14 ? 0x59cf4080 : ciphertext[14]) ^ MAGIC[14];
        result |= (11 < ciphertext.length ? ciphertext[11] : 0xc10685d1) ^ MAGIC[11];
        result |= MAGIC[8] ^ (8 >= ciphertext.length ? 0x7115ff71 : ciphertext[8]);
        result |= (ciphertext.length > 34 ? ciphertext[34] : 0xae295971) ^ MAGIC[34];
        result |= (0 >= ciphertext.length ? 0x973cfb1d : ciphertext[0]) ^ MAGIC[0];
        result |= MAGIC[4] ^ (4 < ciphertext.length ? ciphertext[4] : 0x76803d2a);
        result |= (ciphertext.length <= 15 ? 0x70bd998e : ciphertext[15]) ^ MAGIC[15];
        result |= (13 < ciphertext.length ? ciphertext[13] : 0x231b71a1) ^ MAGIC[13];
        result |= MAGIC[36] ^ (36 >= ciphertext.length ? 0x3dcc6291 : ciphertext[36]);
        result |= (ciphertext.length > 23 ? ciphertext[23] : 0x824c51ed) ^ MAGIC[23];
        result |= (31 >= ciphertext.length ? 0xb8eb036d : ciphertext[31]) ^ MAGIC[31];
        result |= MAGIC[3] ^ (3 < ciphertext.length ? ciphertext[3] : 0x309a54b4);
        result |= (ciphertext.length <= 30 ? 0xbee31b07 : ciphertext[30]) ^ MAGIC[30];
        result |= (19 < ciphertext.length ? ciphertext[19] : 0x601c8ec9) ^ MAGIC[19];
        result |= MAGIC[32] ^ (32 >= ciphertext.length ? 0x0ff4b251 : ciphertext[32]);
        result |= (ciphertext.length > 17 ? ciphertext[17] : 0xd01d3359) ^ MAGIC[17];
        result |= (35 >= ciphertext.length ? 0x24b70270 : ciphertext[35]) ^ MAGIC[35];
        result |= MAGIC[16] ^ (16 < ciphertext.length ? ciphertext[16] : 0xa36e8a79);
        result |= (!(37 < ciphertext.length) ? 0xa38cf9ef : ciphertext[37]) ^ MAGIC[37];
        result |= (6 < ciphertext.length ? ciphertext[6] : 0x63ae859f) ^ MAGIC[6];
        result |= MAGIC[18] ^ (18 >= ciphertext.length ? 0x3223dd35 : ciphertext[18]);
        result |= (ciphertext.length > 28 ? ciphertext[28] : 0xc5c270f4) ^ MAGIC[28];
        result |= (5 >= ciphertext.length ? 0xfb968795 : ciphertext[5]) ^ MAGIC[5];
        result |= MAGIC[1] ^ (1 < ciphertext.length ? ciphertext[1] : 0x8b750c84);
        result |= (!(20 < ciphertext.length) ? 0xa05b70e3 : ciphertext[20]) ^ MAGIC[20];
        result |= (26 < ciphertext.length ? ciphertext[26] : 0x8c2e7af9) ^ MAGIC[26];
        result |= MAGIC[2] ^ (2 >= ciphertext.length ? 0x3aff4165 : ciphertext[2]);
        result |= (ciphertext.length > 12 ? ciphertext[12] : 0x6a15db3e) ^ MAGIC[12];
        result |= (10 >= ciphertext.length ? 0x2f67b723 : ciphertext[10]) ^ MAGIC[10];
        result |= MAGIC[21] ^ (21 < ciphertext.length ? ciphertext[21] : 0x09a65a83);

        return result == 0;
    }

    /** @noinspection PointlessArithmeticExpression, ManualArrayCopy */
    @NonNull
    public byte[] encrypt(@NonNull byte[] plaintext) {
        a = 0xf2463b22aafafae0L;
        b = 0x66a68c0ac00c3379L;

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
            for (int k = 0; k < BLOCK_SIZE; k++) {
                ciphertext[i + k] ^= prev[k];
            }

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

    @NonNull
    public byte[] generate(int n) {
        final byte[] out = new byte[n];

        for (int i = 0; i < n; i += Long.BYTES) {
            b = a + b;
            a = b - a;

            for (int j = 0; j < Long.BYTES && i + j < n; j++) {
                out[i + j] = (byte) (b >> (((Long.BYTES - 1) - j) * Byte.SIZE));
            }
        }

        return out;
    }

    @NonNull
    public byte[] decrypt(@NonNull byte[] ciphertext) {
        a = 0xf2463b22aafafae0L;
        b = 0x66a68c0ac00c3379L;

        byte[] block;
        byte[] prev = generate(BLOCK_SIZE);
        byte[] plaintext = new byte[ciphertext.length];

        for (int i = 0; i < ciphertext.length; i += BLOCK_SIZE) {
            block = Arrays.copyOfRange(ciphertext, i, i + BLOCK_SIZE);

            block = dec(block);
            xor(block, prev);
            prev = Arrays.copyOfRange(ciphertext, i, i + BLOCK_SIZE);

            System.arraycopy(block, 0, plaintext, i, BLOCK_SIZE);
        }

        return unpad(plaintext);
    }

    @NonNull
    public byte[] dec(@NonNull byte[] ciphertext) {
        assert ciphertext.length == BLOCK_SIZE;

        final byte[][] keys = new byte[ROUNDS][];
        for (int i = 0; i < ROUNDS; i++) {
            keys[i] = generate(KEY_SIZE);
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
    public byte[] unpad(@NonNull byte[] text) {
        final int pad = text[text.length - 1];
        return Arrays.copyOfRange(text, 0, text.length - pad);
    }

    public static void xor(@NonNull byte[] x, @NonNull byte[] y) {
        for (int k = 0; k < x.length; k++) {
            x[k] = xor(x[k], y[k]);
        }
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
