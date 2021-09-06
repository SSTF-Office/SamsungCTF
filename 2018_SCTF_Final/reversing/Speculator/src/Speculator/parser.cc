#include "parser.hh"
#include <cstdlib>
#include <byteswap.h>

uint32_t Next4Bytes(ifstream &f){
    uint32_t v = 0;
    f.read((char *)&v, 4);
    if (f.gcount() != 4) {
        fprintf(stdout, "Error: cannot read from input file\n");
        exit(-1);
    }
    return __bswap_32(v);
}
