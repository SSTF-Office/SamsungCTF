#define NUM_REGS    8
#define SIZE  1024 * 1024

/* * * * * * * * * * * * * * * * * *
 * Parsing Part Start              *
 * * * * * * * * * * * * * * * * * */
typedef struct {
    char **instrList;
    int nInstr;
} ParsedInstr;

typedef struct {
    char *dataList;
    int nData;
} ParsedData;

typedef struct {
    ParsedInstr *pRawInstr;
    ParsedData *pRawData;
} Parsed;
/* * * * * * * * * * * * * * * * * *
 * Parsing Part End                *
 * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * *
 * Context Part Start              *
 * * * * * * * * * * * * * * * * * */
typedef enum {
    OP0, // NOP
    OP1, // HALT
    OP2, // INP
    OP3, // OUTP
    OP4, // ADD
    OP5, // SUB
    OP6, // SHL
    OP7, // SHR
    OP8, // AND
    OP9, // OR
    OP10, // XOR
    OP11, // NOT
    OP12, // MOVE
    OP13, // JUMP
    OP14, // SUBLEQ
    OP15 // SUBEQ
} Opcode;

typedef enum {
    REG,
    MEM,
    CONST
} OperandType;

typedef struct {
    bool isUsed;
    OperandType ty;
    int idx;
    bool isIndirect;
} Operand;

typedef struct {
    bool isCached;
    char *rawInstr;
    Opcode opcode;
    Operand operand[3];
} Instr;

typedef struct {
    int ip;
    int regs[NUM_REGS];
    Instr **text;
    int nInstr;
    char *data;
} Context;
/* * * * * * * * * * * * * * * * * *
 * Context Part End                *
 * * * * * * * * * * * * * * * * * */
