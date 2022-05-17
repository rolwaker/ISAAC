#ifndef ISAAC64
#define ISAAC64 1
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct rndstate
{
    uint64_t _randrsl[256];
    uint64_t _mem[256];
    uint64_t _a, _b, _c, _cnt;
    
    rndstate(uint64_t seed);
    void _update();
    
    uint64_t next()
    {
        uint64_t val = this->_randrsl[this->_cnt++];
        
        if (this->_cnt == 256)
            this->_update();
        
        return val;
    }
    
    uint64_t next(uint64_t max)
    {
        uint64_t end = (~(uint64_t)0 / max) * max;
        uint64_t r;
        
        do
            r = this->next();
        while (r >= end);
        
        return r % max;
    }
    
    uint64_t next(uint64_t min, uint64_t max)
    {
        return min + this->next(max - min);
    }
};

#endif  /* RAND */
