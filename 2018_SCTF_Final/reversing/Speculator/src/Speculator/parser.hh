#ifndef MIPS_PARSER
#include <stdint.h>
#include <cstdio>
#include <fstream>
#define MIPS_PARSER

using namespace std;

union Format
{
    uint32_t raw;
    struct
    {
        uint32_t funct : 5;
        uint32_t shamt : 5;
        uint32_t rd : 6;
        uint32_t rt : 6;
        uint32_t rs : 6;
        uint32_t op : 4;
    } R;
    struct
    {
        uint32_t imm : 16;
        uint32_t rt : 6;
        uint32_t rs : 6;
        uint32_t op : 4;
    } I;
    struct
    {
        uint32_t jump_target : 28;
        uint32_t op : 4;
    } J;
};

uint32_t Next4Bytes(ifstream &f);
#endif
