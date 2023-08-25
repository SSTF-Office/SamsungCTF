package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public interface Verifier {
    boolean verify(@NonNull byte[] plaintext);
}
