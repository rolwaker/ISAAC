#include "isaac64.hpp"

rndstate::rndstate(uint64_t seed)
{
    uint64_t arr[8];
    
    for (int i = 0; i < 256; i++) {
        this->_randrsl[i] = seed; // todo better seeding
        this->_mem[i] = 0;
    }
    
    auto mix = [](uint64_t (&arr)[8]) {
        arr[0] -= arr[4]; arr[5] ^= arr[7] >> 9;  arr[7] += arr[0];
        arr[1] -= arr[5]; arr[6] ^= arr[0] << 9;  arr[0] += arr[1];
        arr[2] -= arr[6]; arr[7] ^= arr[1] >> 23; arr[1] += arr[2];
        arr[3] -= arr[7]; arr[0] ^= arr[2] << 15; arr[2] += arr[3];
        arr[4] -= arr[0]; arr[1] ^= arr[3] >> 14; arr[3] += arr[4];
        arr[5] -= arr[1]; arr[2] ^= arr[4] << 20; arr[4] += arr[5];
        arr[6] -= arr[2]; arr[3] ^= arr[5] >> 17; arr[5] += arr[6];
        arr[7] -= arr[3]; arr[4] ^= arr[6] << 14; arr[6] += arr[7];
    };
    
    this->_a = this->_b = this->_c = 0;
    
    for (int i = 0; i < 8; i++)
        arr[i] = 0x9E3779B97F4A7C13LL; // the golden ratio
    
    for (int i = 0; i < 4; i++)
        mix(arr);
    
    for (int i = 0; i < 256; i += 8) { // fill in mm[] with messy stuff
        for (int j = 0; j < 8; j++) // use all the information in the seed
            arr[j] += this->_randrsl[i + j];
        
        mix(arr);
        
        for (int j = 0; j < 8; j++)
            this->_mem[i + j] = arr[j];
    }
    
    // do arr[0] second pass to make all of the seed affect all of mm[]
    for (int i = 0; i < 256; i += 8) {
        for (int j = 0; j < 8; j++)
            arr[j] += this->_mem[i + j];
        
        mix(arr);
        
        for (int j = 0; j < 8; j++)
            this->_mem[i + j] = arr[j];
    }
    
    this->_update();
}

void rndstate::_update()
{
    uint64_t a, b, x, y, *m, *m2, *r, *mend;
    
    auto rngstep = [&](uint64_t mix) {
        x = *m;
        a = mix + *m2++;
        *m++ = y = this->_mem[((x) & (255 << 3)) >> 3] + a + b;
        *r++ = b = this->_mem[((y >> 8) & (255 << 3)) >> 3] + x;
    };
    
    m = this->_mem;
    r = this->_randrsl;
    
    a = this->_a;
    b = this->_b + ++this->_c;
    mend = m2 = this->_mem + 128;
    
    while (m < mend) {
        rngstep(~(a ^ (a << 21)));
        rngstep(a ^ (a >> 5));
        rngstep(a ^ (a << 12));
        rngstep(a ^ (a >> 33));
    }
    
    m2 = this->_mem;
    
    while (m2 < mend) {
        rngstep(~(a ^ (a << 21)));
        rngstep(a ^ (a >> 5));
        rngstep(a ^ (a << 12));
        rngstep(a ^ (a >> 33));
    }
    
    this->_b = b;
    this->_a = a;
    
    this->_cnt = 0;
}
