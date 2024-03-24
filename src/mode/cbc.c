#include "mode/cbc.h"
#include "encryption.h"


/*
    [Cipher Block Chaining Mode]
    @param input length :: length in bytes 
*/

void cbc(BYTE key[32], BYTE* input, size_t length){

    BYTE initialization_vector[16];
    init_iv(initialization_vector);

    ((WORD*)input)[0] ^= ((WORD*)initialization_vector)[0];
    ((WORD*)input)[1] ^= ((WORD*)initialization_vector)[1];
    ((WORD*)input)[2] ^= ((WORD*)initialization_vector)[2];
    ((WORD*)input)[3] ^= ((WORD*)initialization_vector)[3];

    encrypt(input, key);

    for(;length>0; length-=16)
    {
        input += 16;
        ((WORD*)input)[0] ^= ((WORD*)input)[-4];
        ((WORD*)input)[1] ^= ((WORD*)input)[-3];
        ((WORD*)input)[2] ^= ((WORD*)input)[-2];
        ((WORD*)input)[3] ^= ((WORD*)input)[-1];

        encrypt(input, key);
    }
        

}

