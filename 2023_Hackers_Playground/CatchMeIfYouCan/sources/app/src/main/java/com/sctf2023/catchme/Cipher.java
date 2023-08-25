package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public interface Cipher {
    @NonNull
    byte[] encrypt(@NonNull byte[] plaintext);
}
