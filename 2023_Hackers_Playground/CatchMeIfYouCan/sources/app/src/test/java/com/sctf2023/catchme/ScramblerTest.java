package com.sctf2023.catchme;

import androidx.annotation.NonNull;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;
import java.util.Random;

import static java.nio.charset.StandardCharsets.UTF_8;
import static org.junit.Assert.*;

public class ScramblerTest {
    public static final String FLAG = "SCTF{FR4NK_L00K_N0B0DY_15_CH451N6_Y0U}";
    public static final byte[] MAGIC = {77, -87, 98, 90, -123, -118, 25, 41, -73, 0, 88, -71, 2, -42, 17, -49, -97, -111, -2, -94, -88, -77, 85, 32, 73, -32, 2, -124, 58, 110, 84, 121, 106, 51, 95, 63, 31, -33, -103, -56};
    private MyScrambler cipher;

    @Before
    public void setUp() throws Exception {
        cipher = new MyScrambler();
    }

    @Test
    public void test() {
        final byte[] decrypted = cipher.decrypt(MAGIC);
        System.out.println(new String(decrypted, UTF_8));
    }

    @Test
    public void testFlag() {
//        final Random random = new Random();
//        for (int i = 0; i < MAGIC.length; i++) {
//            System.out.printf("%#010x\n", random.nextInt());
//        }

        final String plaintext = FLAG;
        final byte[] bytes = plaintext.getBytes(UTF_8);
        final byte[] ciphertext = cipher.encrypt(bytes);
        final byte[] decrypted = cipher.decrypt(ciphertext);

        System.out.printf("byte[] MAGIC = %s;\n", Arrays.toString(ciphertext).replace('[', '{').replace(']', '}'));

        assertEquals(plaintext, new String(decrypted, UTF_8));
        assertArrayEquals(MAGIC, ciphertext);
    }

    @Test
    public void testScrambler() {
        assertTrue(new Scrambler().verify(FLAG.getBytes(UTF_8)));
        assertFalse(new Scrambler().verify("SCTF{not_a_flag_not_a_flag}".getBytes(UTF_8)));
        assertFalse(new Scrambler().verify("SCTF{not_a_flag_not_a_flag_not_a_flag_not_a_flag_not_a_flag_not_a_flag_not_a_flag_not_a_flag}".getBytes(UTF_8)));
    }

//    @Test
    public void testBruteForce() {
        final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{_}";

        byte[] input = "SCTF{*************************************".getBytes();
        byte[] encrypted = cipher.encrypt(input);
        int count_now = checker(encrypted);

        System.out.println(new String(input) + " : " + hex(encrypted) + " : " + count_now + " : " + hex(MAGIC));

        while (true) {
            boolean find = false;

            for (int n = 5; n < MAGIC.length; n++) {
                int count;
                int count_now_temp = -1;

                for (int i = 0; i < ALPHABET.length(); i++) {
                    byte x = (byte) ALPHABET.charAt(i);
                    byte temp = input[n];

                    input[n] = x;
                    encrypted = cipher.encrypt(input);
                    count = checker(encrypted);

                    System.out.println(new String(input) + ": " + hex(encrypted) + " : " + count + " : " + hex(MAGIC));

                    if (count == count_now + 1) {
                        count_now_temp = count;
                        find = true;
                        System.out.println("Found");
                        break;
                    } else {
                        input[n] = temp;
                    }
                }

                if (count_now_temp != -1) {
                    count_now = count_now_temp;
                }
            }

            if (!find) break;
        }

        System.out.println(new String(input));
    }

    private static int checker(byte[] input) {
        int result = 0;
        for (int i = 0; i < MAGIC.length && i < input.length; i++) {
            if (input[i] == MAGIC[i]) {
                result++;
            }
        }
        return result;
    }

    @NonNull
    public static String hex(@NonNull byte[] input) {
        final StringBuilder builder = new StringBuilder();
        for (int i = 0; i < input.length; i++) {
            if (i != 0 && i % MyCipher.BLOCK_SIZE == 0) {
                builder.append('-');
            }
            builder.append(String.format("%02x", input[i]));

        }
        return builder.toString();
    }
}