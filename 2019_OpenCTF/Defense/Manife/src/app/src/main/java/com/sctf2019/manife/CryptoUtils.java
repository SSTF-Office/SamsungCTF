package com.sctf2019.manife;

import androidx.annotation.NonNull;

/**
 * @author vkarko
 * @since 31.05.19 11:08
 */
public class CryptoUtils {
    @NonNull
    public static byte[] xor(@NonNull byte[] x, @NonNull byte[] y) {
        if (!(x.length == y.length)) throw new AssertionError();

        final byte[] bytes = new byte[x.length];
        for (int i = 0; i < bytes.length; i++) {
            bytes[i] = (byte) (x[i] ^ y[i]);
        }

        return bytes;
    }

    @NonNull
    public static byte[] add(@NonNull byte[] x, @NonNull byte[] y) {
        if (!(x.length == y.length)) throw new AssertionError();

        final byte[] bytes = new byte[x.length * 2];

        //noinspection ManualArrayCopy
        for (int i = 0; i < x.length; i++) {
            bytes[i] = x[i];
        }

        //noinspection ManualArrayCopy
        for (int i = 0; i < y.length; i++) {
            bytes[x.length + i] = y[i];
        }

        return bytes;
    }

    @NonNull
    public static String toHex(byte[] bytes) {
        final StringBuilder builder = new StringBuilder();
        for (byte b : bytes) {
            builder.append(String.format("%02x", b));
        }
        return builder.toString();
    }

    @NonNull
    public static byte[] toBytes(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i+1), 16));
        }
        return data;
    }
}
