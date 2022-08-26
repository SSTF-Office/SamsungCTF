/**
 * @see https://www.oryx-embedded.com/doc/base64_8c_source.html
 */

#include "base64.h"

//Base64 decoding table
static const unsigned int base64DecTable[128] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x3E,0xFF,0xFF,0xFF,0x3F,
    0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,
    0x3C,0x3D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
    0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
    0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
    0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
    0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF
};

int base64(lua_State *L, const char *input) {
    unsigned int value = 0;

    int j = 0;
    int pad = 0;

    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '\r' || input[i] == '\n') {
            // Skip CR and LF characters
        } else if (input[i] == '=') {
            pad++;
        } else if (base64DecTable[input[i]] < 64 && pad == 0) {
            value = (value << 6) | base64DecTable[input[i]];

            if (++j == 4) {
                luaL_addchar(&buffer, (value >> 16) & 0xFF);
                luaL_addchar(&buffer, (value >> 8) & 0xFF);
                luaL_addchar(&buffer, value & 0xFF);

                j = 0;
                value = 0;
            }
        } else return -1;
    }

    if (pad == 0 && j == 0) {
        // No pad, skip
    } else if (pad == 1 && j == 3) {
        luaL_addchar(&buffer, (value >> 10) & 0xFF);
        luaL_addchar(&buffer, (value >> 2) & 0xFF);
    } else if (pad == 2 && j == 2) {
        luaL_addchar(&buffer, (value >> 4) & 0xFF);
    } else return -1;

    luaL_pushresult(&buffer);
    return 0;
}
