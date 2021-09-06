package com.sctf2019.manife;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.AbstractCollection;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public class Node<T> extends AbstractCollection<Node<T>> {
    T value;
    Node<T> parent;
    private Collection<Node<T>> children;

    Node(T value, @Nullable Node<T> parent) {
        this.value = value;
        this.parent = parent;
        if (parent == null) return;
        if (parent.children == null) parent.children = new LinkedList<>();
        parent.children.add(this);
    }

    @NonNull
    @Override
    public Iterator<Node<T>> iterator() {
        return children == null ? Collections.<Node<T>>emptyIterator() : children.iterator();
    }

    @Override
    public int size() {
        return children == null ? 0 : children.size();
    }

    @Nullable
    public Node<T> getParent() {
        return parent;
    }

    @NonNull
    @Override
    public String toString() {
        return "<" + value + ">";
    }
}
