#ifdef BTB
#ifndef __BTB__
#define __BTB__
#include <list>
#include <cstdint>
struct BTBEntry
{
    uint32_t valid : 1;
    uint32_t PC : 31;
    uint32_t target : 32;
};

class BTBModule
{
    private:
        struct BTBEntry BTBTbl[32];
    public:
        uint32_t predict(uint32_t pc);
        void commitResult(uint32_t pc, uint32_t result);
};
#endif
#endif
