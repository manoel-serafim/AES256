#include "encryption.h"
#include "key.h"

/*
    [GF(2⁸) Multiplication by 3]
    :: as 0x03 equals to the polynomial x+1, the multiplication is just a shift left opperation and an addition of the polynomial that takes the mod x⁸+x⁴+x³+x+1 when the result polinomial is of degree higher than 7
*/
BYTE gf_mult_0x03(BYTE a) {  /* 3 = 2 + 1 */
    return gf_mult_0x02(a) ^ a;
}

/*
    [Encryption Rounds]
    :: All 14 rounds with all the layers that should be defined
    @param plain :: the address into a 16 byte (128 bit) data vector
    > plain will be turned into the cipher text
    @param key :: 32 bytes (256bit) aes key to be extended and used in the algorithm
*/
void encrypt(BYTE plain[16], BYTE key[32]){

    WORD extended_key[60];
    key_expansion((WORD *)key, extended_key);

    BYTE state_matrix[16];
    BYTE diffused_matrix[16];
    BYTE key_index;
    

    /*
        [Key Addition Layer]
        :: to start, we compute the addition GF(2⁸) of the plaintext and the key
    */
    ((WORD*)plain)[0] = extended_key[0] ^ (WORD)((WORD*)plain)[0];
    ((WORD*)plain)[1] = extended_key[1] ^ (WORD)((WORD*)plain)[1];
    ((WORD*)plain)[2] = extended_key[2] ^ (WORD)((WORD*)plain)[2];
    ((WORD*)plain)[3] = extended_key[3] ^ (WORD)((WORD*)plain)[3];
    
    for(BYTE round = 1; round<15; round++)
    {
        /*  
            [Byte Substitution Layer]
            ::Here we will use our lookup table in order to insert confusion into our algorithm
            ** Access of the confusion matrix provides the byte substitution
            [Row Shifting Layer] 
            :: byte-wise permutation of the state matrix. It is the 1th step in generating diffusion
            ** Index of the state_matrix provides the permutation from the shift row

            > to reduce looping overhead, the definitions will be made from byte-to-byte, this makes the assembly not have to compute at least 4 commands that do nothing for our algorithm (CMP)
                -down side is that the final program binary will be bigger
            > the row shift permutation is done automatically to save on function calling
        */
        state_matrix[0] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[0])][LEAST_SIGNIFICANT_NIBBLE(plain[0])];
        state_matrix[13] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[1])][LEAST_SIGNIFICANT_NIBBLE(plain[1])];
        state_matrix[10] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[2])][LEAST_SIGNIFICANT_NIBBLE(plain[2])];
        state_matrix[7] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[3])][LEAST_SIGNIFICANT_NIBBLE(plain[3])];

        state_matrix[4] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[4])][LEAST_SIGNIFICANT_NIBBLE(plain[4])];
        state_matrix[1] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[5])][LEAST_SIGNIFICANT_NIBBLE(plain[5])];
        state_matrix[14] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[6])][LEAST_SIGNIFICANT_NIBBLE(plain[6])];
        state_matrix[11] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[7])][LEAST_SIGNIFICANT_NIBBLE(plain[7])];

        state_matrix[8] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[8])][LEAST_SIGNIFICANT_NIBBLE(plain[8])];
        state_matrix[5] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[9])][LEAST_SIGNIFICANT_NIBBLE(plain[9])];
        state_matrix[2] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[10])][LEAST_SIGNIFICANT_NIBBLE(plain[10])];
        state_matrix[15] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[11])][LEAST_SIGNIFICANT_NIBBLE(plain[11])];

        state_matrix[12] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[12])][LEAST_SIGNIFICANT_NIBBLE(plain[12])];
        state_matrix[9] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[13])][LEAST_SIGNIFICANT_NIBBLE(plain[13])];
        state_matrix[6] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[14])][LEAST_SIGNIFICANT_NIBBLE(plain[14])];
        state_matrix[3] = confusion_matrix[MOST_SIGNIFICANT_NIBBLE(plain[15])][LEAST_SIGNIFICANT_NIBBLE(plain[15])];
        
        if( round == 14)
        {
            /*
                [Last Round]
                :: When in the last round, we do not compute the mix column
            */
            ((WORD*)diffused_matrix)[0] = (WORD)((WORD*)state_matrix)[0];
            ((WORD*)diffused_matrix)[1] = (WORD)((WORD*)state_matrix)[1];
            ((WORD*)diffused_matrix)[2] = (WORD)((WORD*)state_matrix)[2];
            ((WORD*)diffused_matrix)[3] = (WORD)((WORD*)state_matrix)[3]; 
            goto key_addition;
        }
        /*
            [Mix Column]
            ::most important step in the dffusion of the algorithm
            A structure of 4 bytes will be multiplied GF(2⁸) into a constant matrix 
            * The XOR Operation is used in order to make the dot product of the diffusion matrix:
                [Mix Column Diffusion Matrix]
                :: will be used to multiply the extended galois field elements
                static const BYTE diffusion_matrix[4][4] = 
                { 
                    0x02, 0x03, 0x01, 0x01,
                    0x01, 0x02, 0x03, 0x01,
                    0x01, 0x01, 0x02, 0x03,
                    0x03, 0x01, 0x01, 0x02
                };

            > to reduce looping assembly overhead, the definitions will be made from byte-to-byte
            (This will make the assembly 4 instructions shorter at a minimum [depending on the architecture])

            > the multiplication will be made using lookup tables
                -bigger program
                +faster
            -- will NOT be executed if we are in the last round.
        */
        diffused_matrix[0] = gf_mult_0x02(state_matrix[0]) ^
                            gf_mult_0x03(state_matrix[1]) ^
                            state_matrix[2] ^
                            state_matrix[3];

        diffused_matrix[1] = state_matrix[0] ^
                            gf_mult_0x02(state_matrix[1]) ^  
                            gf_mult_0x03(state_matrix[2]) ^
                            state_matrix[3];

        diffused_matrix[2] = state_matrix[0] ^
                            state_matrix[1] ^
                            gf_mult_0x02(state_matrix[2]) ^ 
                            gf_mult_0x03(state_matrix[3]);
        
        diffused_matrix[3] = gf_mult_0x03(state_matrix[0]) ^
                            state_matrix[1] ^
                            state_matrix[2] ^
                            gf_mult_0x02(state_matrix[3]);

        diffused_matrix[4] = gf_mult_0x02(state_matrix[4]) ^
                            gf_mult_0x03(state_matrix[5]) ^
                            state_matrix[6] ^
                            state_matrix[7];

        diffused_matrix[5] = state_matrix[4] ^
                            gf_mult_0x02(state_matrix[5]) ^  
                            gf_mult_0x03(state_matrix[6]) ^
                            state_matrix[7];

        diffused_matrix[6] = state_matrix[4] ^
                            state_matrix[5] ^
                            gf_mult_0x02(state_matrix[6]) ^ 
                            gf_mult_0x03(state_matrix[7]);

        diffused_matrix[7] = gf_mult_0x03(state_matrix[4]) ^
                            state_matrix[5] ^
                            state_matrix[6] ^
                            gf_mult_0x02(state_matrix[7]);


        diffused_matrix[8] = gf_mult_0x02(state_matrix[8]) ^
                            gf_mult_0x03(state_matrix[9]) ^
                            state_matrix[10] ^
                            state_matrix[11];

        diffused_matrix[9] = state_matrix[8] ^
                            gf_mult_0x02(state_matrix[9]) ^  
                            gf_mult_0x03(state_matrix[10]) ^
                            state_matrix[11];

        diffused_matrix[10] = state_matrix[8] ^
                            state_matrix[9] ^
                            gf_mult_0x02(state_matrix[10]) ^ 
                            gf_mult_0x03(state_matrix[11]);

        diffused_matrix[11] = gf_mult_0x03(state_matrix[8]) ^
                            state_matrix[9] ^
                            state_matrix[10] ^
                            gf_mult_0x02(state_matrix[11]);

        diffused_matrix[12] = gf_mult_0x02(state_matrix[12]) ^
                            gf_mult_0x03(state_matrix[13]) ^
                            state_matrix[14] ^
                            state_matrix[15];

        diffused_matrix[13] = state_matrix[12] ^
                            gf_mult_0x02(state_matrix[13]) ^  
                            gf_mult_0x03(state_matrix[14]) ^
                            state_matrix[15];

        diffused_matrix[14] = state_matrix[12] ^
                            state_matrix[13] ^
                            gf_mult_0x02(state_matrix[14]) ^ 
                            gf_mult_0x03(state_matrix[15]);

        diffused_matrix[15] = gf_mult_0x03(state_matrix[12]) ^
                            state_matrix[13] ^
                            state_matrix[14] ^
                            gf_mult_0x02(state_matrix[15]);
key_addition:
        /*
            [Key Addition Layer]
            :: simple XOR with the generated extended subkey
        */
        key_index = (round<<2);
        ((WORD*)plain)[0] = extended_key[key_index+0] ^ (WORD)((WORD*)diffused_matrix)[0];
        ((WORD*)plain)[1] = extended_key[key_index+1] ^ (WORD)((WORD*)diffused_matrix)[1];
        ((WORD*)plain)[2] = extended_key[key_index+2] ^ (WORD)((WORD*)diffused_matrix)[2];
        ((WORD*)plain)[3] = extended_key[key_index+3] ^ (WORD)((WORD*)diffused_matrix)[3]; 

    }
}

