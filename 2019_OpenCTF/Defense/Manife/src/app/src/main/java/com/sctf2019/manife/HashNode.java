package com.sctf2019.manife;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public class HashNode extends Node<String> {

    public HashNode() {
        super("root", null);
    }

    public HashNode(@NonNull String value, @Nullable Node<String> parent) {
        super(value, parent);
    }

    @Nullable
    @Override
    public HashNode getParent() {
        return (HashNode) super.getParent();
    }
}
