package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public class MyGenerator2 implements Generator {
    private int p;
    private int q;

    public MyGenerator2(int p, int q) {
        this.p = p;
        this.q = q;
    }

    @NonNull
    @Override
    public byte[] generate(int n) {
        assert n > 0;

        final byte[] out = new byte[n];

        for (int i = 0; i < n; i += 4) {
            q = p + q;
            p = q - p;

            for (int j = 0; j < 4 && i + j < n; j++) {
                out[i + j] = (byte) (q >> ((3 - j) * 8));
            }
        }

        return out;
    }
}
