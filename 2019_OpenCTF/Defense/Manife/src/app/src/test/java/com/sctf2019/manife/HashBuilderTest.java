package com.sctf2019.manife;

import org.junit.Test;

import java.security.MessageDigest;

import static org.junit.Assert.assertEquals;

/**
 * @author vkarko
 * @since 30.05.19 18:23
 */
public class HashBuilderTest {

    @Test
    public void test1() throws Exception {
        final HashBuilder builder = new HashBuilder(MessageDigest.getInstance("SHA-256"));

        final HashNode A = new HashNode("A", null);
        final HashNode B = new HashNode("B", A);
        final HashNode C = new HashNode("C", A);
        builder.accept(A);

        assertEquals("df7e70e5021544f4834bbee64a9e3789febc4be81470df629cad6ddb03320a5c", B.value);
        assertEquals("6b23c0d5f35d1b11f9b683f0b0a617355deb11277d91ae091d399c655b87940d", C.value);

        final HashNode A2 = new HashNode("A", null);
        new HashNode("C", A2);
        new HashNode("B", A2);
        builder.accept(A2);

        assertEquals(A.value, A2.value);
    }
}
