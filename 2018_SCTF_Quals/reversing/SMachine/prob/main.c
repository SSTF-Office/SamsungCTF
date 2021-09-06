#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

/* * * * * * * * * * * * * * * * * *
 * Utility Part Start              *
 * * * * * * * * * * * * * * * * * */
void clearAndRead(FILE *fp, char *dst, int size) {
    memset(dst, 0, size);
    fread(dst, 1, size, fp);
}
/* * * * * * * * * * * * * * * * * *
 * Utility Part Start              *
 * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * *
 * Parsing Part Start              *
 * * * * * * * * * * * * * * * * * */
char *readFromFile(char *fileName) {
    FILE *fp = fopen(fileName, "r");

    char header[16];
    clearAndRead(fp, header, 16);
    if (strncmp(header, "SMACHINE", 8)) {
        printf("Invalid File Format\n");
        exit(-1);
    }

    unsigned int size = ((unsigned int *) header)[2];

    char *buf = calloc(size, sizeof(char));
    clearAndRead(fp, buf, size);

    fclose(fp);

    return buf;
}

ParsedInstr *initParsedInstr() {
    ParsedInstr *pRawInstr = calloc(1, sizeof(ParsedInstr));
    pRawInstr->instrList = calloc(SIZE, sizeof(char *));
    pRawInstr->nInstr = SIZE;

    return pRawInstr;
}

ParsedData *initParsedData() {
    ParsedData *pRawData = calloc(1, sizeof(ParsedData));
    pRawData->dataList = calloc(SIZE, sizeof(char));
    pRawData->nData = SIZE;

    return pRawData;
}

int getNumOperandBytes1(unsigned char type0) {
    if (type0 == 1) {
        return 1;
    } else if (type0 == 2) {
        return 3;
    } else if (type0 == 4) {
        return 4;
    } else {
        printf("Invalid Operand Type\n");
        exit(-1);
    }
}

int getNumOperandBytes2(unsigned char type0, unsigned char type1) {
    return getNumOperandBytes1(type0) + getNumOperandBytes1(type1);
}

int getNumOperandBytes3
(unsigned char type0, unsigned char type1, unsigned char type2) {
    return getNumOperandBytes1(type0) + getNumOperandBytes2(type1, type2);
}

Parsed *parseBinary(char *buf) {
    ParsedInstr *pRawInstr = initParsedInstr();
    ParsedData *pRawData = initParsedData();

    int i;
    int cur = 0;
    char *ptr;
    for (i = 0; i < SIZE; i++) {
        unsigned short instr = *((unsigned short *) (buf + cur));

        Opcode opcode = instr & 0xf;
        unsigned char operand0Type;
        unsigned char operand1Type;
        unsigned char operand2Type;
        int nOperandBytes;
        switch (opcode) {
            case OP0:
            case OP1:
                ptr = calloc(2, sizeof(char));
                memcpy(ptr, buf + cur, 2);
                cur += 2;
                break;
            case OP2:
            case OP3:
            case OP13:
                operand0Type = (instr & 0x70) >> 4;
                nOperandBytes = getNumOperandBytes1(operand0Type);
                ptr = calloc(nOperandBytes, sizeof(char));
                memcpy(ptr, buf + cur, 2 + nOperandBytes);
                cur += 2 + nOperandBytes;
                break;
            case OP11:
            case OP12:
                operand0Type = (instr & 0x70) >> 4;
                operand1Type = (instr & 0x380) >> 7;
                nOperandBytes = getNumOperandBytes2(operand0Type, operand1Type);
                ptr = calloc(nOperandBytes, sizeof(char));
                memcpy(ptr, buf + cur, 2 + nOperandBytes);
                cur += 2 + nOperandBytes;
                break;
            case OP4:
            case OP5:
            case OP6:
            case OP7:
            case OP8:
            case OP9:
            case OP10:
            case OP14:
            case OP15:
                operand0Type = (instr & 0x70) >> 4;
                operand1Type = (instr & 0x380) >> 7;
                operand2Type = (instr & 0x1c00) >> 10;
                nOperandBytes = getNumOperandBytes3(operand0Type, operand1Type,
                        operand2Type);
                ptr = calloc(nOperandBytes, sizeof(char));
                memcpy(ptr, buf + cur, 2 + nOperandBytes);
                cur += 2 + nOperandBytes;
                break;
            default:
                printf("Invalid Opcode\n");
                exit(-1);
        }
        pRawInstr->instrList[i] = ptr;

        pRawData->dataList[i] = buf[cur];
        cur++;
    }

    free(buf);

    Parsed *parsed = calloc(1, sizeof(Parsed));
    parsed->pRawInstr = pRawInstr;
    parsed->pRawData = pRawData;

    return parsed;
}

/* * * * * * * * * * * * * * * * * *
 * Parsing Part End                *
 * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * *
 * Context Part Start              *
 * * * * * * * * * * * * * * * * * */
void loadInstr(Context *ctx, char **rawInstrs, int nInstr) {
    int i;
    for (i = 0; i < nInstr; i++) {
        ctx->text[i] = calloc(1, sizeof(Instr));
        ctx->text[i]->isCached = false;
        ctx->text[i]->rawInstr = rawInstrs[i];
    }

    ctx->nInstr = nInstr;
}

void loadData(Context *ctx, char *rawData, int nData) {
    int i;
    for (i = 0; i < nData; i++) {
        ctx->data[i] = rawData[i];
    }
}

Context *initContext(Parsed *parsed) {
    Context *ctx = calloc(1, sizeof(Context));

    ctx->text = calloc(SIZE, sizeof(Instr *));
    loadInstr(ctx, parsed->pRawInstr->instrList, parsed->pRawInstr->nInstr);

    ctx->data = calloc(SIZE * 2, sizeof(char));
    loadData(ctx, parsed->pRawData->dataList, parsed->pRawData->nData);

    free(parsed->pRawInstr->instrList);
    free(parsed->pRawInstr);
    free(parsed->pRawData->dataList);
    free(parsed->pRawData);
    free(parsed);

    return ctx;
}

void setOperand(Operand *operand, unsigned char ty, int *rawInstr) {
    if (ty == 1) {
        operand->ty = REG;
        operand->idx = rawInstr[0] & 0xff;
        if (operand->idx >= NUM_REGS) {
            printf("Register out of range\n");
            exit(-3);
        }
    } else if (ty == 2) {
        operand->ty = MEM;
        operand->idx = rawInstr[0] & 0xffffff;
    } else if (ty == 4) {
        operand->ty = CONST;
        operand->idx = rawInstr[0];
    }
}

Instr *fetchInstr(Context *ctx) {
    if (ctx->ip >= ctx->nInstr) {
        printf("Instruction out of range\n");
        exit(-3);
    }

    Instr *target = ctx->text[ctx->ip];

    if (target->isCached) {
        ctx->ip++;

        return target;
    }

    unsigned short instr = *((unsigned short *) target->rawInstr);
    target->opcode = instr & 0xf;
    unsigned char operand0Type;
    unsigned char operand1Type;
    unsigned char operand2Type;
    unsigned char isIndirect;
    int *rawPtr;
    switch (target->opcode) {
            case OP0:
            case OP1:
                target->operand[0].isUsed = false;
                target->operand[1].isUsed = false;
                target->operand[2].isUsed = false;
                break;
            case OP2:
            case OP3:
            case OP13:
                target->operand[0].isUsed = true;
                isIndirect = (instr & 0xe000) >> 13;
                operand0Type = (instr & 0x70) >> 4;
                rawPtr = (int *) (target->rawInstr + 2);
                setOperand(&target->operand[0], operand0Type, rawPtr);
                if ((isIndirect & 0x1) == 0x1) {
                    target->operand[0].isIndirect = true;
                } else {
                    target->operand[0].isIndirect = false;
                }
                target->operand[1].isUsed = false;
                target->operand[2].isUsed = false;
                break;
            case OP11:
            case OP12:
                target->operand[0].isUsed = true;
                isIndirect = (instr & 0xe000) >> 13;
                operand0Type = (instr & 0x70) >> 4;
                rawPtr = (int *) (target->rawInstr + 2);
                setOperand(&target->operand[0], operand0Type, rawPtr);
                if ((isIndirect & 0x1) == 0x1) {
                    target->operand[0].isIndirect = true;
                } else {
                    target->operand[0].isIndirect = false;
                }
                target->operand[1].isUsed = true;
                operand1Type = (instr & 0x380) >> 7;
                if (target->operand[0].ty == REG) {
                    rawPtr = (int *) (((char *) rawPtr) + 1);
                } else if (target->operand[0].ty == MEM) {
                    rawPtr = (int *) (((char *) rawPtr) + 3);
                } else {
                    rawPtr = (int *) (((char *) rawPtr) + 4);
                }
                setOperand(&target->operand[1], operand1Type, rawPtr);
                if ((isIndirect & 0x2) == 0x2) {
                    target->operand[1].isIndirect = true;
                } else {
                    target->operand[1].isIndirect = false;
                }
                target->operand[2].isUsed = false;
                break;
            case OP4:
            case OP5:
            case OP6:
            case OP7:
            case OP8:
            case OP9:
            case OP10:
            case OP14:
            case OP15:
                target->operand[0].isUsed = true;
                isIndirect = (instr & 0xe000) >> 13;
                operand0Type = (instr & 0x70) >> 4;
                rawPtr = (int *) (target->rawInstr + 2);
                setOperand(&target->operand[0], operand0Type, rawPtr);
                if ((isIndirect & 0x1) == 0x1) {
                    target->operand[0].isIndirect = true;
                } else {
                    target->operand[0].isIndirect = false;
                }
                target->operand[1].isUsed = true;
                operand1Type = (instr & 0x380) >> 7;
                if (target->operand[0].ty == REG) {
                    rawPtr = (int *) (((char *) rawPtr) + 1);
                } else if (target->operand[0].ty == MEM) {
                    rawPtr = (int *) (((char *) rawPtr) + 3);
                } else {
                    rawPtr = (int *) (((char *) rawPtr) + 4);
                }
                setOperand(&target->operand[1], operand1Type, rawPtr);
                if ((isIndirect & 0x2) == 0x2) {
                    target->operand[1].isIndirect = true;
                } else {
                    target->operand[1].isIndirect = false;
                }
                target->operand[2].isUsed = true;
                operand2Type = (instr & 0x1c00) >> 10;
                if (target->operand[1].ty == REG) {
                    rawPtr = (int *) (((char *) rawPtr) + 1);
                } else if (target->operand[1].ty == MEM) {
                    rawPtr = (int *) (((char *) rawPtr) + 3);
                } else {
                    rawPtr = (int *) (((char *) rawPtr) + 4);
                }
                setOperand(&target->operand[2], operand2Type, rawPtr);
                if ((isIndirect & 0x4) == 0x4) {
                    target->operand[2].isIndirect = true;
                } else {
                    target->operand[2].isIndirect = false;
                }
                break;
            default:
                printf("Invalid Opcode\n");
                exit(-1);
    }

    ctx->ip++;

    return target;
}

int get_value_from_memory(Context *ctx, int addr) {
    if (addr > 0x1000000) {
        printf("Address out of range\n");
        exit(-3);
    }

    return ((int *) &ctx->data[addr])[0];
}

void set_value_to_memory(Context *ctx, int addr, int value, int size) {
    if (addr > 0x1000000) {
        printf("Address out of range\n");
        exit(-3);
    }

    if (size == 1) {
        ctx->data[addr] = (char) value;
    } else if (size == 4) {
        ((int *) &ctx->data[addr])[0] = value;
    } else {
        printf("Invalid writing size\n");
        exit(-3);
    }
}

void execute_input(Context *ctx, Operand *operand) {
    ctx->regs[0] = getchar() & 0xFF;

    if (operand->ty == REG) {
        if (operand->idx == 0 || operand->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand->idx], ctx->regs[0], 1);
        } else {
            ctx->regs[operand->idx] = ctx->regs[0];
        }
    } else if (operand->ty == MEM) {
        if (operand->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand->idx),
                    ctx->regs[0], 1);
        } else {
            set_value_to_memory(ctx, operand->idx, ctx->regs[0], 1);
        }
    } else {
        if (operand->isIndirect) {
            set_value_to_memory(ctx, operand->idx, ctx->regs[0], 1);
        } else {
            printf("Invalid operand type\n");
            exit(-3);
        }
    }

    ctx->regs[0] = 0;
}

void execute_output(Context *ctx, Operand *operand) {
    if (operand->ty == REG) {
        if (operand->idx == 0 || operand->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand->isIndirect) {
            ctx->regs[1] =
                get_value_from_memory(ctx, ctx->regs[operand->idx]) & 0xff;
        } else {
            ctx->regs[1] = ctx->regs[operand->idx] & 0xff;
        }
    } else if (operand->ty == MEM) {
        if (operand->isIndirect) {
            ctx->regs[1] = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand->idx)) & 0xff;
        } else {
            ctx->regs[1] = get_value_from_memory(ctx, operand->idx) & 0xff;
        }
    } else {
        if (operand->isIndirect) {
            ctx->regs[1] = get_value_from_memory(ctx, operand->idx) & 0xff;
        } else {
            printf("Invalid operand type\n");
            exit(-3);
        }
    }

    printf("%c", (char) ctx->regs[1]);

    ctx->regs[1] = 0;
}

void execute_add
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result += get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result += ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result += get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result += get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result += get_value_from_memory(ctx, operand2->idx);
        } else {
            result += operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_sub
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result -= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result -= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result -= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result -= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result -= get_value_from_memory(ctx, operand2->idx);
        } else {
            result -= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_shl
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result <<= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result <<= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result <<= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result <<= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result <<= get_value_from_memory(ctx, operand2->idx);
        } else {
            result <<= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_shr
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result >>= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result >>= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result >>= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result >>= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result >>= get_value_from_memory(ctx, operand2->idx);
        } else {
            result >>= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_and
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result &= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result &= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result &= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result &= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result &= get_value_from_memory(ctx, operand2->idx);
        } else {
            result &= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_or
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result |= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result |= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result |= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result |= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result |= get_value_from_memory(ctx, operand2->idx);
        } else {
            result |= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_xor
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            result ^= get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            result ^= ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            result ^= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            result ^= get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            result ^= get_value_from_memory(ctx, operand2->idx);
        } else {
            result ^= operand2->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_not(Context *ctx, Operand *operand0, Operand *operand1) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result = ~get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ~ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = ~get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = ~get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = ~get_value_from_memory(ctx, operand1->idx);
        } else {
            result = ~operand1->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_move(Context *ctx, Operand *operand0, Operand *operand1) {
    int result;

    if (operand1->ty == REG) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result = ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result = get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result = get_value_from_memory(ctx, operand1->idx);
        } else {
            result = operand1->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }
}

void execute_jump(Context *ctx, Operand *operand0) {
    int addr;
    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            addr = get_value_from_memory(ctx, ctx->regs[operand0->idx]);
        } else {
            addr = ctx->regs[operand0->idx];
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            addr = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand0->idx));
        } else {
            addr = get_value_from_memory(ctx, operand0->idx);
        }
    } else {
        if (operand0->isIndirect) {
            addr = get_value_from_memory(ctx, operand0->idx);
        } else {
            addr = operand0->idx;
        }
    }

    ctx->ip = addr;
}

void execute_subleq
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand0->idx]);
        } else {
            result = ctx->regs[operand0->idx];
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand0->idx));
        } else {
            result = get_value_from_memory(ctx, operand0->idx);
        }
    } else {
        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx, operand0->idx);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result -= ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result -= get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx, operand1->idx);
        } else {
            result -= operand1->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    }

    int addr;
    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            addr = ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            addr = get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx, operand2->idx);
        } else {
            addr = operand2->idx;
        }
    }

    if (result <= 0) {
        ctx->ip = addr;
    }
}

void execute_subeq
(Context *ctx, Operand *operand0, Operand *operand1, Operand *operand2) {
    int result;

    if (operand0->ty == REG) {
        if (operand0->idx == 0 || operand0->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx, ctx->regs[operand0->idx]);
        } else {
            result = ctx->regs[operand0->idx];
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand0->idx));
        } else {
            result = get_value_from_memory(ctx, operand0->idx);
        }
    } else {
        if (operand0->isIndirect) {
            result = get_value_from_memory(ctx, operand0->idx);
        } else {
            printf("Invalid operand type is used\n");
            exit(-3);
        }
    }

    if (operand1->ty == REG) {
        if (operand1->idx == 0 || operand1->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx, ctx->regs[operand1->idx]);
        } else {
            result -= ctx->regs[operand1->idx];
        }
    } else if (operand1->ty == MEM) {
        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand1->idx));
        } else {
            result -= get_value_from_memory(ctx, operand1->idx);
        }
    } else {
        if (operand1->isIndirect) {
            result -= get_value_from_memory(ctx, operand1->idx);
        } else {
            result -= operand1->idx;
        }
    }

    if (operand0->ty == REG) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, ctx->regs[operand0->idx], result, 4);
        } else {
            ctx->regs[operand0->idx] = result;
        }
    } else if (operand0->ty == MEM) {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, get_value_from_memory(ctx, operand0->idx),
                    result, 4);
        } else {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    } else {
        if (operand0->isIndirect) {
            set_value_to_memory(ctx, operand0->idx, result, 4);
        }
    }

    int addr;
    if (operand2->ty == REG) {
        if (operand2->idx == 0 || operand2->idx == 1) {
            printf("Invalid register is used\n");
            exit(-3);
        }

        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx, ctx->regs[operand2->idx]);
        } else {
            addr = ctx->regs[operand2->idx];
        }
    } else if (operand2->ty == MEM) {
        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx,
                    get_value_from_memory(ctx, operand2->idx));
        } else {
            addr = get_value_from_memory(ctx, operand2->idx);
        }
    } else {
        if (operand2->isIndirect) {
            addr = get_value_from_memory(ctx, operand2->idx);
        } else {
            addr = operand2->idx;
        }
    }

    if (result == 0) {
        ctx->ip = addr;
    }
}

bool execute(Context *ctx, Instr *instr) {
    switch (instr->opcode) {
        case OP0:
            return false;
        case OP1:
            return true;
        case OP2:
            execute_input(ctx, &instr->operand[0]);
            return false;
        case OP3:
            execute_output(ctx, &instr->operand[0]);
            return false;
        case OP4:
            execute_add(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP5:
            execute_sub(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP6:
            execute_shl(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP7:
            execute_shr(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP8:
            execute_and(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP9:
            execute_or(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP10:
            execute_xor(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP11:
            execute_not(ctx, &instr->operand[0], &instr->operand[1]);
            return false;
        case OP12:
            execute_move(ctx, &instr->operand[0], &instr->operand[1]);
            return false;
        case OP13:
            execute_jump(ctx, &instr->operand[0]);
            return false;
        case OP14:
            execute_subleq(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        case OP15:
            execute_subeq(ctx, &instr->operand[0], &instr->operand[1], &instr->operand[2]);
            return false;
        default:
            return false;
    }
}
/* * * * * * * * * * * * * * * * * *
 * Context Part End                *
 * * * * * * * * * * * * * * * * * */

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [Binary]\n", argv[0]);
        exit(-1);
    }

    char *binary = readFromFile(argv[1]);
    Parsed *parsed = parseBinary(binary);

    Context *ctx = initContext(parsed);

    while (true) {
        Instr *instr = fetchInstr(ctx);
        bool isFinished = execute(ctx, instr);

        if (isFinished) {
            break;
        }
    }

    return 0;
}
