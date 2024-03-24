#include "mode/ctr.h"
#include "encryption.h"
#include "decryption.h"


/*
    [Counter Mode]
    @param  length :: length in bytes 
*/

void ctr_encrypt(
    BYTE key[32], 
    BYTE* block, 
    size_t length)
{

    BYTE initialization_vector[16];
    /*
        Implement LFSR 
    */
    WORD counter;

    for(counter = 0xFFFFFFFF; length>0; counter++,block += 16,length -= 16;)
    {
        if(couter == 0xFFFFFFFF)
        {
            init_iv(initialization_vector);
            counter = 0x00000000;
        }
        ((WORD*)initialization_vector)[3] = counter;
        encrypt(block,key);

        ((WORD*)block)[0] ^= ((WORD*)initialization_vector)[0];
        ((WORD*)block)[1] ^= ((WORD*)initialization_vector)[1];
        ((WORD*)block)[2] ^= ((WORD*)initialization_vector)[2];
        ((WORD*)block)[3] ^= ((WORD*)initialization_vector)[3];
    }
    

}


/*
    [Cipher Block Chaining Mode]
    @param  length :: length in bytes 
*/
void ctr_decrypt(
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
