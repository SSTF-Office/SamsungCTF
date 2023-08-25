package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public class MyGenerator implements Generator {
    private long p;
    private long q;

    public MyGenerator(long p, long q) {
        this.p = p;
        this.q = q;
    }

    @NonNull
    @Override
    public byte[] generate(int n) {
        final byte[] out = new byte[n];

        for (int i = 0; i < n; i += Long.BYTES) {
            q = p + q;
            p = q - p;

            for (int j = 0; j < Long.BYTES && i + j < n; j++) {
                out[i + j] = (byte) (q >> (((Long.BYTES - 1) - j) * Byte.SIZE));
            }
        }

        return out;
    }
}
