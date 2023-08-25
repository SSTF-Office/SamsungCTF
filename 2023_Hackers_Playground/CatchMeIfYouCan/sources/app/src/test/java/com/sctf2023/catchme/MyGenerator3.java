package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public class MyGenerator3 implements Generator {
    private int p;
    private int q;

    public MyGenerator3(int p, int q) {
        this.p = p;
        this.q = q;
    }

    @NonNull
    @Override
    public byte[] generate(int n) {
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
