#ifndef __MEMORY__
#define __MEMORY__
#include <map>
#include <list>
#include <cstdint>
#include "Opcode.hh"

#define PAGE_SIZE 0x1000
#define PERM_READ 0x1
#define PERM_WRITE 0x2
#define PERM_EXEC 0x4

struct Page {
    uint32_t value[PAGE_SIZE/4];
    char perm;
    bool priv;
};

struct CacheEntry {
    bool dirty;
    uint32_t addr;
    uint32_t value;
    char perm;
};

class MemoryModule
{
    private:
        std::map<uint32_t, Page *> m;
#ifdef CACHE
        std::list<CacheEntry *> LRUCache;
#endif
        struct Page *FindMem(uint32_t addr);
        unsigned char *GetPtr(uint32_t addr);
        unsigned char *GetPtr(uint32_t addr, char perm, bool priv, uint32_t *cause, bool *epc_write);
    public:
        void AllocMem(uint32_t addr, bool priv, char perm);
        void WriteMem(uint32_t addr, uint32_t value);
        uint32_t ReadMem(uint32_t addr);

        void WriteMem(uint32_t addr, uint32_t value, bool priv, uint32_t *cause, bool *epc_write);
        uint32_t ReadMem(uint32_t addr, bool inst, bool priv, uint32_t *cause, bool *epc_write);
#ifdef CACHE
        void LoadCache(uint32_t addr, bool priv, uint32_t *cause, bool *epc_write);
        uint32_t ReadCache(uint32_t addr, uint32_t *cause, bool *epc_write);
        void WriteCache(uint32_t addr, uint32_t value, uint32_t *cause, bool *epc_write);
#endif

};
#endif
