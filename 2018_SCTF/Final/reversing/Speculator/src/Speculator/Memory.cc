#include "Memory.hh"
#include <cstring>
#include <iostream>

void MemoryModule::AllocMem(uint32_t addr, bool priv, char perm) {
    struct Page *p = (struct Page *)malloc(sizeof(struct Page));
    memset(p->value, 0, PAGE_SIZE);
    p->priv = priv;
    p->perm = perm;
    m[addr] = p;
}

struct Page *MemoryModule::FindMem(uint32_t addr)
{
    auto it = m.find(addr & 0xfffff000);
    if (it == m.end()) {
        printf("Memory error\n");
        exit(0);
    }
    return it->second;
}

unsigned char *MemoryModule::GetPtr(uint32_t addr)
{
    struct Page *p = FindMem(addr);
    return (unsigned char *)(p->value) + (addr & 0xfff);
}

void MemoryModule::WriteMem(uint32_t addr, uint32_t value)
{
    *(this->GetPtr(addr + 0)) = (value >> 24) & 0xff;
    *(this->GetPtr(addr + 1)) = (value >> 16) & 0xff;
    *(this->GetPtr(addr + 2)) = (value >> 8) & 0xff;
    *(this->GetPtr(addr + 3)) = (value >> 0) & 0xff;
}

unsigned char *MemoryModule::GetPtr(uint32_t addr, char perm, bool priv,
        uint32_t *cause, bool *epc_write)
{
    struct Page *p = FindMem(addr);
    if(p == NULL || (p->perm & perm) != perm || (p->priv && !priv))
    {
        *cause = INT_SEGFAULT;
        *epc_write = true;
        return 0;
    }
    return (unsigned char *)(p->value) + (addr & 0xfff);
}

void MemoryModule::WriteMem(uint32_t addr, uint32_t value, bool priv,
        uint32_t *cause, bool *epc_write)
{
    //printf("Writing %08x at %08x\n", value, addr);
    if(addr & 3)
    {
        *epc_write = true;
        *cause = INT_BUSERR;
    }
    else
    {
        for(int i = 0; i < 4; i++)
        {
            unsigned char* ptr = GetPtr(addr + i, PERM_WRITE, priv, cause, epc_write);
            if (*epc_write) return;
            *ptr = (value >> (8 * (3-i))) & 0xff;
        }
    }
}
uint32_t MemoryModule::ReadMem(uint32_t addr)
{
    return
        (*(this->GetPtr(addr + 0)) << 24) |
        (*(this->GetPtr(addr + 1)) << 16) |
        (*(this->GetPtr(addr + 2)) << 8) |
        (*(this->GetPtr(addr + 3)) << 0);
}

uint32_t MemoryModule::ReadMem(uint32_t addr, bool inst, bool priv, uint32_t *cause, bool *epc_write)
{
    if(addr & 3)
    {
        *epc_write = true;
        *cause = INT_BUSERR;
        return 0;
    }
    else
    {
        int perm = inst ? PERM_EXEC | PERM_READ : PERM_READ;
        unsigned char* b0 = this->GetPtr(addr + 0, perm, priv, cause, epc_write);
        unsigned char* b1 = this->GetPtr(addr + 1, perm, priv, cause, epc_write);
        unsigned char* b2 = this->GetPtr(addr + 2, perm, priv, cause, epc_write);
        unsigned char* b3 = this->GetPtr(addr + 3, perm, priv, cause, epc_write);
        if (!b0 || !b1 || !b2 || !b3)
            return 0;
        uint32_t word = ((*b0) << 24) | ((*b1) << 16) | ((*b2) << 8) | (*b3);
        //if (!inst) printf("Reading %08x (priv=%d): %08x\n", addr, priv, word);
        return word;
    }
}

#ifdef CACHE
void MemoryModule::LoadCache(uint32_t addr, bool priv, uint32_t *cause, bool *epc_write)
{
#ifdef DEBUG
    printf("LoadCache %x priv=%d\n", addr, priv);
#endif
    if(addr & 3)
    {
        *epc_write = true;
        *cause = INT_BUSERR;
        return;
    }
    else
    {
        auto it = LRUCache.begin();
        for(; it != LRUCache.end(); it++)
        {
            struct CacheEntry *entry = *it;
            if (entry->addr == addr)
            {
                //printf("Found %x in cache\n", addr);
                LRUCache.erase(it);
                LRUCache.push_back(entry);
                return;
            }
        }
        // NOT FOUND
        if(LRUCache.size() > 32)
        {
            struct CacheEntry *entry = LRUCache.front();
            if(entry->dirty)
                WriteMem(entry->addr, entry->value);
            LRUCache.pop_front();
            free(entry);
        }
        uint32_t val = ReadMem(addr, 0, priv, cause, epc_write);
        if(!(*epc_write))
        {
            struct CacheEntry *entry = (struct CacheEntry *)malloc(sizeof(struct CacheEntry));
            entry->dirty = false;
            entry->addr = addr;
            entry->value = val;
            struct Page *p = FindMem(addr);
            entry->perm = p->perm;
            LRUCache.push_back(entry);
        }
    }
}

uint32_t MemoryModule::ReadCache(uint32_t addr, uint32_t *cause, bool *epc_write)
{
    auto it = LRUCache.begin();
    for(; it != LRUCache.end(); it++)
    {
        struct CacheEntry *entry = *it;
        if (entry->addr == addr)
        {
            if(entry->perm & PERM_READ)
                return entry->value;
            else
            {
                *cause = INT_SEGFAULT;
                *epc_write = true;
                return 0;
            }
        }
    }
    std::cerr << "Cache error" << std::endl;
    exit(-1);
}

void MemoryModule::WriteCache(uint32_t addr, uint32_t value, uint32_t * cause, bool *epc_write)
{
    auto it = LRUCache.begin();
    for(; it != LRUCache.end(); it++)
    {
        struct CacheEntry *entry = *it;
        if (entry->addr == addr)
        {
            if(entry->perm & PERM_WRITE)
            {
                entry->value = value;
                entry->dirty = true;
            }
            else
            {
                *cause = INT_SEGFAULT;
                *epc_write = true;
            }
            return;
        }
    }
    std::cerr << "Cache error" << std::endl;
    exit(-1);
}

#endif
