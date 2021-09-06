package com.sctf2019.manife;

import androidx.annotation.NonNull;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * @author vkarko
 * @since 30.05.19 18:29
 */
public class VisitorTest {
    private Node<String> root;

    private class MockVisitor extends Visitor<Node<String>> {
        private StringBuilder builder = new StringBuilder();

        MockVisitor(@NonNull Node<String> node) {
            accept(node);
        }

        @Override
        public boolean onVisit(@NonNull Node<String> node) {
            builder.append(node.value);
            return true;
        }

        @NonNull
        @Override
        public String toString() {
            return builder.toString();
        }
    }

    @Before
    public void setUp() {
        root = new Node<>("A", null);

        final Node<String> B = new Node<>("B", root);
        new Node<>("D", B);
        new Node<>("F", B);

        final Node<String> C = new Node<>("C", root);
        new Node<>("G", C);

        new Node<>("E", root);
    }

    @Test
    public void test1() {
        assertEquals("DFBGCEA", new MockVisitor(root).toString());
    }
}
