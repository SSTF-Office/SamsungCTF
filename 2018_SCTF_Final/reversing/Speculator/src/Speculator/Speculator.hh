#ifndef MIPS_EMUL
#define MIPS_EMUL

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <iostream>
#include <fstream>

#include "parser.hh"
#include "Opcode.hh"
#include "Memory.hh"
#include "BTB.hh"

using namespace std;

#define DATA_START 0x10000000
#define CODE_START 0x400000
#define REG_NUM 64


typedef enum {L_PC = 0x1, L_IF_ID = 0x2, L_ID_EX = 0x4, L_EX_MEM = 0x8,
              L_MEM_WB = 0x10, L_ALL = 0x1f} latch;
typedef enum {ADD, AND, OR, SLT, SLL, SRL, SUB, XOR, EQ, NEQ,
              LUI, NAN, EXIT} ALU_OP;
typedef enum {aREG, aIMM} ALU_SRC;
typedef enum {P01, P02, P03} port;
typedef enum {ALU_result, Read_Data} MTR;
typedef enum {NA = 0, JUMP, EXCEPTION} pTarget;

struct _Forwarding {
    port MUX1;
    port MUX2;
};

struct _Hazard {
    int ex_stall;
};

struct _pc {
    bool valid;
    uint32_t pc;

    pTarget pPort;
    uint32_t btarget;
    uint32_t jtarget;
};

struct _IF_ID {
    bool valid;
    uint32_t pc;

    uint32_t inst;
    uint32_t NPC;
    uint32_t INT_CAUSE;
    bool EPC_WRITE;
};

struct _ID_EX {
    bool valid;
    uint32_t pc;

    uint32_t NPC;

    uint32_t reg1;
    uint8_t reg1num;
    uint32_t reg2;
    uint8_t reg2num;

    uint32_t imm;
    uint32_t shamt;

    ALU_OP ALUOp;
    ALU_SRC ALUSrc;

    bool MemRead;
    bool MemWrite;
#ifdef BTB
    uint32_t Prediction;
#endif
    uint32_t saved_pc;

    bool RegWrite;
    uint8_t RegDst;
    MTR MemToReg;

    uint32_t INT_CAUSE;
    bool EPC_WRITE;
};

struct _EX_MEM {
    bool valid;
    uint32_t pc;

    uint32_t ALU_OUT;
    uint32_t BR_TARGET;

    uint32_t wdata;

    bool MemRead;
    bool MemWrite;
    uint32_t saved_pc;
    bool branch;
    bool branchop;

    bool RegWrite;
    uint8_t RegDst;
#ifdef BTB
    uint32_t Prediction;
#endif
    MTR MemToReg;
    bool exited;

    uint32_t INT_CAUSE;
    bool EPC_WRITE;
};

struct _MEM_WB {
    bool valid;
    uint32_t pc;

    uint32_t ALU_OUT;
    uint32_t MEM_OUT;

    bool RegWrite;
    uint8_t RegDst;
    MTR MemToReg;

    bool EPC_WRITE;
    uint32_t epc;
    bool priv;
};

class Speculator
{
    protected:
        MemoryModule Memory;
#ifdef DEBUG
        bool dFlag, pFlag, mFlag;
        uint32_t st, ed;
#endif
        uint8_t FLUSH;
        uint32_t saved_pc;
        uint32_t cycle;
#ifdef BTB
        BTBModule BTBM;
#endif
        struct _pc PC;
        struct _IF_ID IF_ID;
        struct _ID_EX ID_EX;
        struct _EX_MEM EX_MEM;
        struct _MEM_WB MEM_WB;

        struct _Hazard Hazard;
        struct _Forwarding Forward;

        uint32_t R[REG_NUM];
        uint32_t epc;
        bool priv;

        uint32_t IMSIZE;

        void SelectPC(struct _pc *pc_buffer);
        void IFetch(struct _IF_ID *L);
        void Dec(struct _pc *PL, struct _ID_EX *L);
        void Execute(struct _EX_MEM *L);
        void MemAccess(struct _pc *pc_buf, struct _MEM_WB *L);
        void WriteBack();
        void Update(struct _pc pc, struct _IF_ID L1, struct _ID_EX L2, struct _EX_MEM L3, struct _MEM_WB L4);

        uint32_t readReg(uint8_t num);
        void setReg(uint8_t num, uint32_t val);

    public:
        Speculator(string kern, string bin);
        MemoryModule& getMemory() { return Memory; };

        void WriteMem(uint32_t addr, uint32_t value);
        uint32_t ReadMem(uint32_t addr);

        void load(ifstream &f, bool priv);
        void run();

        void InstructionError();
        void Halt(string err);

        void AddTreasure(string fl);
};

#endif

