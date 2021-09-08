#ifdef BTB
#include "BTB.hh"
#include <stdio.h>
#define MASK 0x7fffffff
uint32_t BTBModule::predict(uint32_t pc)
{
    uint32_t key = pc & MASK;
    uint32_t idx = (pc >> 2) & 31;
    if(BTBTbl[idx].valid && BTBTbl[idx].PC == key)
    {
        uint32_t target = BTBTbl[idx].target;
#ifdef DEBUG
        printf("PREDICT %x -> %x\n", pc, target);
#endif
        return target;
    }
    return pc + 4;
}

void BTBModule::commitResult(uint32_t pc, uint32_t result)
{
    uint32_t key = pc & MASK;
    uint32_t idx = (pc >> 2) & 31;
    BTBTbl[idx].valid = 1;
    BTBTbl[idx].PC = key;
    BTBTbl[idx].target = result;
#ifdef DEBUG
    printf("UPDATE %x->%x\n", pc, result);
#endif
}
#endif
