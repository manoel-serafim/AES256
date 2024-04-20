#include "mode/cbc.h"
#include "encryption.h"
#include "decryption.h"


/*
    [Cipher Block Chaining Mode]
    @param  key: 256 bit key
    @param  length :: length in bytes 
    @param  block :: 
*/

void cbc_encrypt(
    BYTE key[32], 
    BYTE* block, 
    size_t length)
{

    BYTE initialization_vector[16];
    init_iv(initialization_vector);

    ((WORD*)block)[0] ^= ((WORD*)initialization_vector)[0];
    ((WORD*)block)[1] ^= ((WORD*)initialization_vector)[1];
    ((WORD*)block)[2] ^= ((WORD*)initialization_vector)[2];
    ((WORD*)block)[3] ^= ((WORD*)initialization_vector)[3];

    encrypt(block, key);

    while(length>0)
    {
        block += 16;
        length -= 16;
        ((WORD*)block)[0] ^= ((WORD*)block)[-4];
        ((WORD*)block)[1] ^= ((WORD*)block)[-3];
        ((WORD*)block)[2] ^= ((WORD*)block)[-2];
        ((WORD*)block)[3] ^= ((WORD*)block)[-1];

        encrypt(block, key);
    }
        
}


/*
    [Cipher Block Chaining Mode]
    @param  length :: length in bytes 
*/
void cbc_decrypt(
    BYTE key[32], 
    BYTE initialization_vector[16], 
    BYTE* block, 
    size_t length)
{
    decrypt(block, key);
    
    ((WORD*)block)[0] ^= ((WORD*)initialization_vector)[0];
    ((WORD*)block)[1] ^= ((WORD*)initialization_vector)[1];
    ((WORD*)block)[2] ^= ((WORD*)initialization_vector)[2];
    ((WORD*)block)[3] ^= ((WORD*)initialization_vector)[3];


    while(length>0)
    {
        block += 16;
        length -= 16;
        decrypt(block, key);
        ((WORD*)block)[0] ^= ((WORD*)block)[-4];
        ((WORD*)block)[1] ^= ((WORD*)block)[-3];
        ((WORD*)block)[2] ^= ((WORD*)block)[-2];
        ((WORD*)block)[3] ^= ((WORD*)block)[-1];

        
    }
        
}
