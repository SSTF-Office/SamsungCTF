package com.sctf2023.catchme;

import androidx.annotation.NonNull;

public interface Generator {
    @NonNull
    byte[] generate(int n);
}
