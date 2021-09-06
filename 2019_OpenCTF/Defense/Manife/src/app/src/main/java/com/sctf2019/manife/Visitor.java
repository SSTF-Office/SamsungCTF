package com.sctf2019.manife;

import androidx.annotation.NonNull;

import java.util.Deque;
import java.util.LinkedList;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public abstract class Visitor<T extends Node<?>> {
    private final Deque<T> stack = new LinkedList<>();
    private final Deque<T> order = new LinkedList<>();

    public boolean accept(@NonNull T node) {
        stack.push(node);

        T current;
        while (!stack.isEmpty()) {
            current = stack.pop();
            order.push(current);

            for (Node<?> kid : current) {
                //noinspection unchecked
                stack.push((T) kid);
            }
        }

        while (!order.isEmpty()) {
            if (!onVisit(order.pop())) return false;
        }

        return true;
    }

    protected abstract boolean onVisit(@NonNull T node);
}
