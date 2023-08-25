package com.sctf2023.catchme;

import androidx.annotation.NonNull;
import com.sctf2023.exceptions.*;

/** @noinspection CaughtExceptionImmediatelyRethrown, InfiniteLoopStatement, PointlessArithmeticExpression */
public class Scrambler implements Verifier {
    private static final int BLOCK_SIZE = 4;
    private static final int KEY_SIZE = 6;
    private static final int ROUNDS = 9;

    private long p;
    private long q;

    @Override
    public boolean verify(@NonNull byte[] plaintext) {
        byte[] key = new byte[0];
        byte[] out = new byte[0];
        byte[] ciphertext = new byte[0];
        byte[] prev = new byte[BLOCK_SIZE];
        byte a = 0, b = 0, c = 0, d = 0, f = 0;
        byte pad = 0;
        int i = 0, j = 0, k = 0;
        int result = 0;

        try {
            Exception ip = null;
            while (true) {
                try {
                    try {
                        throw ip;
                    }
                    catch (NullPointerException e) {
                        try {
                            throw new E1();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            p = 0xf2463b22aafafae0L;
                            q = 0x66a68c0ac00c3379L;
                        }
                    }
                    catch (E1 e) {
                        try {
                            try {
                                throw e.getCause();
                            }
                            catch (NullPointerException unused) {
                                throw new E2();
                            }
                            catch (E17 unused) {
                                throw new E18();
                            }
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            q = p + q;
                            p = q - p;
                        }
                    }
                    catch (E2 e) {
                        try {
                            try {
                                throw e.getCause();
                            }
                            catch (E1 unused) {
                                throw new E3();
                            }
                            catch (E7 unused) {
                                throw new E8();
                            }
                            catch (E8 unused) {
                                throw new E10();
                            }
                            finally {
                                i = 0;
                            }
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E3 e) {
                        try {
                            throw i < BLOCK_SIZE ? new E4() : new E6();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E4 e) {
                        try {
                            throw new E5();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            prev[i] = (byte) (q >> (((Long.BYTES - 1) - i) * Byte.SIZE));
                        }
                    }
                    catch (E5 e) {
                        try {
                            try {
                                throw e.getCause();
                            } catch (E4 e2) {
                                throw new E3();
                            } catch (E9 e2) {
                                throw new E8();
                            } finally {
                                i++;
                            }
                        }
                        catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E6 e) {
                        try {
                            throw new E7();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            pad = (byte) (BLOCK_SIZE - plaintext.length % BLOCK_SIZE);
                        }
                    }
                    catch (E7 e) {
                        try {
                            throw new E2();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            ciphertext = new byte[plaintext.length + pad];
                        }
                    }
                    catch (E8 e) {
                        try {
                            throw i < ciphertext.length ? new E9() : new E2();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E9 e) {
                        try {
                            throw new E5();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            ciphertext[i] = i < plaintext.length ? plaintext[i] : pad;
                        }
                    }
                    catch (E10 e) {
                        try {
                            throw i < plaintext.length ? new E11() : new E43();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E11 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E10 | E15 e2) {
                            throw new E12().initCause(e);
                        }
                        catch (E12 e2) {
                            throw new E15().initCause(e);
                        }
                        finally {
                            j = 0;
                        }
                    }
                    catch (E12 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E11 | E14 e2) {
                            try {
                                throw e2.getCause();
                            }
                            catch (E10 | E13 e3) {
                                try {
                                    throw j < BLOCK_SIZE ? new E13() : new E11();
                                } catch (Exception e4) {
                                    throw e4.initCause(e);
                                }
                            }
                            catch (E15 | E41 e3) {
                                try {
                                    throw j < BLOCK_SIZE ? new E41() : new E42();
                                } catch (Exception e4) {
                                    throw e4.initCause(e);
                                }
                            }
                        }
                    }
                    catch (E13 e) {
                        try {
                            throw new E14();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            ciphertext[i + j] ^= prev[j];
                        }
                    }
                    catch (E14 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E13 | E41 e2) {
                            throw new E12().initCause(e);
                        }
                        catch (E39 e2) {
                            throw new E15().initCause(e);
                        }
                        finally {
                            j++;
                        }
                    }
                    catch (E15 e) {
                        try {
                            throw j < ROUNDS ? new E16() : new E11();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E16 e) {
                        try {
                            throw new E17();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            out = new byte[BLOCK_SIZE];
                        }
                    }
                    catch (E17 e) {
                        try {
                            throw new E1();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            key = new byte[KEY_SIZE];
                        }
                    }
                    catch (E18 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E1 e2) {
                            throw new E19().initCause(e);
                        }
                        catch (E38 e2) {
                            throw new E39().initCause(e);
                        }
                        finally {
                            k = 0;
                        }
                    }
                    catch (E19 e) {
                        try {
                            throw k < KEY_SIZE ? new E20() : new E22();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E20 e) {
                        try {
                            throw new E21();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            key[k] = (byte) (q >> (((Long.BYTES - 1) - k) * Byte.SIZE));
                        }
                    }
                    catch (E21 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E20 e2) {
                            throw new E19().initCause(e);
                        }
                        catch (E40 e2) {
                            throw new E39().initCause(e);
                        }
                        finally {
                            k++;
                        }
                    }
                    catch (E22 e) {
                        try {
                            throw new E23();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            a = (byte) (ciphertext[i + 0] ^ key[0]);
                        }
                    }
                    catch (E23 e) {
                        try {
                            throw new E24();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            b = (byte) (ciphertext[i + 1] ^ key[1]);
                        }
                    }
                    catch (E24 e) {
                        try {
                            throw e.getCause();
                        }
                        catch (E23 e2) {
                            throw new E25().initCause(e);
                        }
                        catch (E29 e2) {
                            throw new E30().initCause(e);
                        }
                        catch (E34 e2) {
                            throw new E35().initCause(e);
                        }
                        finally {
                            c = (byte) (a + b);
                        }
                    }
                    catch (E25 e) {
                        try {
                            throw new E26();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            d = (byte) ((c & 0xff) << 7);
                        }
                    }
                    catch (E26 e) {
                        try {
                            throw new E27();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            f = (byte) ((c & 0xff) >>> 1);
                        }
                    }
                    catch (E27 e) {
                        try {
                            throw new E28();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            out[0] = (byte) (d | f);
                        }
                    }
                    catch (E28 e) {
                        try {
                            throw new E29();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            a = (byte) (ciphertext[i + 1] ^ key[2]);
                        }
                    }
                    catch (E29 e) {
                        try {
                            throw new E24();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            b = (byte) (ciphertext[i + 2] ^ key[3]);
                        }
                    }
                    catch (E30 e) {
                        try {
                            throw new E31();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            d = (byte) ((c & 0xff) >>> 5);
                        }
                    }
                    catch (E31 e) {
                        try {
                            throw new E32();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            f = (byte) ((c & 0xff) << 3);
                        }
                    }
                    catch (E32 e) {
                        try {
                            throw new E33();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            out[1] = (byte) (d | f);
                        }
                    }
                    catch (E33 e) {
                        try {
                            throw new E34();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            a = (byte) (ciphertext[i + 2] ^ key[4]);
                        }
                    }
                    catch (E34 e) {
                        try {
                            throw new E24();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            b = (byte) (ciphertext[i + 3] ^ key[5]);
                        }
                    }
                    catch (E35 e) {
                        try {
                            throw new E36();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            d = (byte) ((c & 0xff) << 3);
                        }
                    }
                    catch (E36 e) {
                        try {
                            throw new E37();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            f = (byte) ((c & 0xff) >>> 5);
                        }
                    }
                    catch (E37 e) {
                        try {
                            throw new E38();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            out[2] = (byte) (d | f);
                        }
                    }
                    catch (E38 e) {
                        try {
                            throw new E18();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            out[3] = ciphertext[i + 0];
                        }
                    }
                    catch (E39 e) {
                        try {
                            throw k < BLOCK_SIZE ? new E40() : new E14();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        }
                    }
                    catch (E40 e) {
                        try {
                            throw new E21();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            ciphertext[i + k] = out[k];
                        }
                    }
                    catch (E41 e) {
                        try {
                            throw new E14();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            prev[j] = ciphertext[i + j];
                        }
                    }
                    catch (E42 e) {
                        try {
                            throw new E10();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            i += BLOCK_SIZE;
                        }
                    }
                    catch (E43 e) {
                        try {
                            throw new E44();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result = ciphertext.length ^ 40;
                        }
                    }
                    catch (E44 e) {
                        try {
                            throw new E45();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -124 ^ (27 >= ciphertext.length ? 0x7a476371 : ciphertext[27]);
                        }
                    }
                    catch (E45 e) {
                        try {
                            throw new E46();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 7 ? ciphertext[7] : 0x78e9a0e1) ^ 41;
                        }
                    }
                    catch (E46 e) {
                        try {
                            throw new E47();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (9 >= ciphertext.length ? 0x04a8628d : ciphertext[9]) ^ 0;
                        }
                    }
                    catch (E47 e) {
                        try {
                            throw new E48();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -103 ^ (38 < ciphertext.length ? ciphertext[38] : 0xe91a88d0);
                        }
                    }
                    catch (E48 e) {
                        try {
                            throw new E49();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length <= 33 ? 0xd3da28f8 : ciphertext[33]) ^ 51;
                        }
                    }
                    catch (E49 e) {
                        try {
                            throw new E50();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (22 < ciphertext.length ? ciphertext[22] : 0x5e508e98) ^ 85;
                        }
                    }
                    catch (E50 e) {
                        try {
                            throw new E51();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= 73 ^ (24 >= ciphertext.length ? 0x00d2f59f : ciphertext[24]);
                        }
                    }
                    catch (E51 e) {
                        try {
                            throw new E52();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 39 ? ciphertext[39] : 0xdc794b90) ^ -56;
                        }
                    }
                    catch (E52 e) {
                        try {
                            throw new E53();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (29 >= ciphertext.length ? 0x90778847 : ciphertext[29]) ^ 110;
                        }
                    }
                    catch (E53 e) {
                        try {
                            throw new E54();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -32 ^ (25 < ciphertext.length ? ciphertext[25] : 0xe293836a);
                        }
                    }
                    catch (E54 e) {
                        try {
                            throw new E55();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length <= 14 ? 0x59cf4080 : ciphertext[14]) ^ 17;
                        }
                    }
                    catch (E55 e) {
                        try {
                            throw new E56();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (11 < ciphertext.length ? ciphertext[11] : 0xc10685d1) ^ -71;
                        }
                    }
                    catch (E56 e) {
                        try {
                            throw new E57();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -73 ^ (8 >= ciphertext.length ? 0x7115ff71 : ciphertext[8]);
                        }
                    }
                    catch (E57 e) {
                        try {
                            throw new E58();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 34 ? ciphertext[34] : 0xae295971) ^ 95;
                        }
                    }
                    catch (E58 e) {
                        try {
                            throw new E59();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (0 >= ciphertext.length ? 0x973cfb1d : ciphertext[0]) ^ 77;
                        }
                    }
                    catch (E59 e) {
                        try {
                            throw new E60();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -123 ^ (4 < ciphertext.length ? ciphertext[4] : 0x76803d2a);
                        }
                    }
                    catch (E60 e) {
                        try {
                            throw new E61();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length <= 15 ? 0x70bd998e : ciphertext[15]) ^ -49;
                        }
                    }
                    catch (E61 e) {
                        try {
                            throw new E62();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (13 < ciphertext.length ? ciphertext[13] : 0x231b71a1) ^ -42;
                        }
                    }
                    catch (E62 e) {
                        try {
                            throw new E63();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= 31 ^ (36 >= ciphertext.length ? 0x3dcc6291 : ciphertext[36]);
                        }
                    }
                    catch (E63 e) {
                        try {
                            throw new E64();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 23 ? ciphertext[23] : 0x824c51ed) ^ 32;
                        }
                    }
                    catch (E64 e) {
                        try {
                            throw new E65();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (31 >= ciphertext.length ? 0xb8eb036d : ciphertext[31]) ^ 121;
                        }
                    }
                    catch (E65 e) {
                        try {
                            throw new E66();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= 90 ^ (3 < ciphertext.length ? ciphertext[3] : 0x309a54b4);
                        }
                    }
                    catch (E66 e) {
                        try {
                            throw new E67();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length <= 30 ? 0xbee31b07 : ciphertext[30]) ^ 84;
                        }
                    }
                    catch (E67 e) {
                        try {
                            throw new E68();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (19 < ciphertext.length ? ciphertext[19] : 0x601c8ec9) ^ -94;
                        }
                    }
                    catch (E68 e) {
                        try {
                            throw new E69();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= 106 ^ (32 >= ciphertext.length ? 0x0ff4b251 : ciphertext[32]);
                        }
                    }
                    catch (E69 e) {
                        try {
                            throw new E70();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 17 ? ciphertext[17] : 0xd01d3359) ^ -111;
                        }
                    }
                    catch (E70 e) {
                        try {
                            throw new E71();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (35 >= ciphertext.length ? 0x24b70270 : ciphertext[35]) ^ 63;
                        }
                    }
                    catch (E71 e) {
                        try {
                            throw new E72();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -97 ^ (16 < ciphertext.length ? ciphertext[16] : 0xa36e8a79);
                        }
                    }
                    catch (E72 e) {
                        try {
                            throw new E73();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (!(37 < ciphertext.length) ? 0xa38cf9ef : ciphertext[37]) ^ -33;
                        }
                    }
                    catch (E73 e) {
                        try {
                            throw new E74();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (6 < ciphertext.length ? ciphertext[6] : 0x63ae859f) ^ 25;
                        }
                    }
                    catch (E74 e) {
                        try {
                            throw new E75();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -2 ^ (18 >= ciphertext.length ? 0x3223dd35 : ciphertext[18]);
                        }
                    }
                    catch (E75 e) {
                        try {
                            throw new E76();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 28 ? ciphertext[28] : 0xc5c270f4) ^ 58;
                        }
                    }
                    catch (E76 e) {
                        try {
                            throw new E77();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (5 >= ciphertext.length ? 0xfb968795 : ciphertext[5]) ^ -118;
                        }
                    }
                    catch (E77 e) {
                        try {
                            throw new E78();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -87 ^ (1 < ciphertext.length ? ciphertext[1] : 0x8b750c84);
                        }
                    }
                    catch (E78 e) {
                        try {
                            throw new E79();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (!(20 < ciphertext.length) ? 0xa05b70e3 : ciphertext[20]) ^ -88;
                        }
                    }
                    catch (E79 e) {
                        try {
                            throw new E80();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (26 < ciphertext.length ? ciphertext[26] : 0x8c2e7af9) ^ 2;
                        }
                    }
                    catch (E80 e) {
                        try {
                            throw new E81();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= 98 ^ (2 >= ciphertext.length ? 0x3aff4165 : ciphertext[2]);
                        }
                    }
                    catch (E81 e) {
                        try {
                            throw new E82();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (ciphertext.length > 12 ? ciphertext[12] : 0x6a15db3e) ^ 2;
                        }
                    }
                    catch (E82 e) {
                        try {
                            throw new E83();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= (10 >= ciphertext.length ? 0x2f67b723 : ciphertext[10]) ^ 88;
                        }
                    }
                    catch (E83 e) {
                        try {
                            throw new E84();
                        } catch (Exception e2) {
                            throw e2.initCause(e);
                        } finally {
                            result |= -77 ^ (21 < ciphertext.length ? ciphertext[21] : 0x09a65a83);
                        }
                    }
                    catch (E84 e) {
                        throw new InterruptedException();
                    }
                }
                catch (InterruptedException e) {
                    throw e;    // <- exit loop
                }
                catch (Exception e) {
                    ip = e;
                }
            }
        }
        catch (Exception e) {
            return result == 0;
        }
        catch (Throwable e) {
            return false;
        }
    }

}
