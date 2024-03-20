#ifndef _TYPES_H_
#define _TYPES_H_

#include <stddef.h>

#define BYTE unsigned char
#define WORD unsigned int

typedef struct {
    unsigned char _ : 4;
}NIBBLE;

#define MOST_SIGNIFICANT_NIBBLE(x) ((x)>>4)
#define LEAST_SIGNIFICANT_NIBBLE(x) ((x)&0xF)

inline BYTE gf_mult_0x02(BYTE x) {
    /*
        [Multiplication by 2]
        :: mutltiplication by two in GF(2⁸) is fairly simple
        we just need to do a shift accompanied by a modulo reduction when needed
        :: it will be used as a building block to construct all other mutiplicators
    */
    return (x << 1) ^ (0x11b & -(x >> 7));
}

#endif /* _TYPES_H_ */