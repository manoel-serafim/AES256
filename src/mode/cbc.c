#include "mode/cbc.h"

void cbc(BYTE key[32], BYTE block[16]){
    BYTE initialization_vector[16];
    init_iv(initialization_vector);

    (WORD*)block ^= (WORD*)initialization_vector[0] 
}

