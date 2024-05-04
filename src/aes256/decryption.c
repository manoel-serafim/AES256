#include "aes256/decryption.h"
#include "aes256/key.h"

/*
    [Byte Inverse Substitution Lookup Table]
    :: allows for returning to the previous structure
    :: Here the new values are the indexes of the confusion_matrix
*/
const BYTE coherence_matrix[16][16] = {
    {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};


/*
    [Multiplication GF(2⁸)]
    :: Is done this way to evade possible timing attacks
    :: will be used in the inverse diffusion
*/
static inline BYTE gf_mult_0x09(BYTE a) 
{  /* 9 = 8 + 1 */
    return gf_mult_0x02(gf_mult_0x02(gf_mult_0x02(a))) ^ a;
}
static inline BYTE gf_mult_0x0B(BYTE a) 
{  /* 11 = 8 + 2 + 1 */
    return gf_mult_0x02(gf_mult_0x02(gf_mult_0x02(a))) ^ gf_mult_0x02(a) ^ a;
}
static inline BYTE gf_mult_0x0D(BYTE a) 
{  /* 13 = 8 + 4 + 1 */
    return gf_mult_0x02(gf_mult_0x02(gf_mult_0x02(a))) ^ gf_mult_0x02(gf_mult_0x02(a)) ^ a;
}
static inline BYTE gf_mult_0x0E(BYTE a) 
{  /* 14 = 8 + 4 + 2 */
    return gf_mult_0x02(gf_mult_0x02(gf_mult_0x02(a))) ^ gf_mult_0x02(gf_mult_0x02(a)) ^ gf_mult_0x02(a);
}

/*
    [Decryption]
    :: AES is not based on a Feistel network, this means all layers must actually be inverted

*/
void decrypt(
    BYTE cipher[16], 
    BYTE key[32])
{

    WORD extended_key[60];
    key_expansion((WORD *)key, extended_key);

    BYTE controled_matrix[16];
    BYTE key_index;
    
    for(BYTE round = 14; round>0; round--)
    {

        key_index = (round<<2);
        /*
            [Key Addition(Subtraction) Layer]
            :: Continues the same as the invert of XOR is the XOR
        */
        ((WORD*)cipher)[0] ^= extended_key[key_index+0];
        ((WORD*)cipher)[1] ^= extended_key[key_index+1];
        ((WORD*)cipher)[2] ^= extended_key[key_index+2];
        ((WORD*)cipher)[3] ^= extended_key[key_index+3];
        
        if( round != 14)
        {
            /*
                [Inverse Mix Column]
                ::most important step to control the diffusion in the cipher text
                A structure of 4 bytes will be multiplied GF(2⁸) into a constant matrix 
                * The XOR Operation is used in order to make the dot product of the diffusion matrix:
                    [Mix Column Control Matrix]
                    :: will be used to multiply the extended galois field elements
                    static const BYTE control_matrix[4][4] = 
                    { 
                      
                        0x0E 0x0B 0x0D 0x09
                        0x09 0x0E 0x0B 0x0D
                        0x0D 0x09 0x0E 0x0B
                        0x0B 0x0D 0x09 0x0E
                    };

                > to reduce looping assembly overhead, the definitions will be made from byte-to-byte
                (This will make the assembly 4 instructions shorter at a minimum [depending on the architecture])

                > the multiplication will be made using lookup tables by 9, 11, 13 and 14 
                    -bigger program
                    +faster
                -- will NOT be executed if we are in the first round.
            */
            
            

            controled_matrix[0] = gf_mult_0x0E(cipher[0]) ^  
                                gf_mult_0x0B(cipher[1]) ^
                                gf_mult_0x0D(cipher[2]) ^
                                gf_mult_0x09(cipher[3]);
         
            controled_matrix[1] = gf_mult_0x09(cipher[0]) ^
                                gf_mult_0x0E(cipher[1]) ^
                                gf_mult_0x0B(cipher[2]) ^ 
                                gf_mult_0x0D(cipher[3]);

            controled_matrix[2] = gf_mult_0x0D(cipher[0]) ^
                                gf_mult_0x09(cipher[1]) ^  
                                gf_mult_0x0E(cipher[2]) ^
                                gf_mult_0x0B(cipher[3]);

            controled_matrix[3] = gf_mult_0x0B(cipher[0]) ^
                                gf_mult_0x0D(cipher[1]) ^
                                gf_mult_0x09(cipher[2]) ^
                                gf_mult_0x0E(cipher[3]);

            controled_matrix[4] = gf_mult_0x0E(cipher[4]) ^  
                                gf_mult_0x0B(cipher[5]) ^
                                gf_mult_0x0D(cipher[6]) ^
                                gf_mult_0x09(cipher[7]);
                    
            controled_matrix[5] = gf_mult_0x09(cipher[4]) ^
                                gf_mult_0x0E(cipher[5]) ^
                                gf_mult_0x0B(cipher[6]) ^ 
                                gf_mult_0x0D(cipher[7]);

            controled_matrix[6] = gf_mult_0x0D(cipher[4]) ^
                                gf_mult_0x09(cipher[5]) ^  
                                gf_mult_0x0E(cipher[6]) ^
                                gf_mult_0x0B(cipher[7]);

            controled_matrix[7] = gf_mult_0x0B(cipher[4]) ^
                                gf_mult_0x0D(cipher[5]) ^
                                gf_mult_0x09(cipher[6]) ^
                                gf_mult_0x0E(cipher[7]);


            controled_matrix[8] = gf_mult_0x0E(cipher[8]) ^  
                                gf_mult_0x0B(cipher[9]) ^
                                gf_mult_0x0D(cipher[10]) ^
                                gf_mult_0x09(cipher[11]);
                    
            controled_matrix[9] = gf_mult_0x09(cipher[8]) ^
                                gf_mult_0x0E(cipher[9]) ^
                                gf_mult_0x0B(cipher[10]) ^ 
                                gf_mult_0x0D(cipher[11]);

            controled_matrix[10] = gf_mult_0x0D(cipher[8]) ^
                                gf_mult_0x09(cipher[9]) ^  
                                gf_mult_0x0E(cipher[10]) ^
                                gf_mult_0x0B(cipher[11]);

            controled_matrix[11] = gf_mult_0x0B(cipher[8]) ^
                                gf_mult_0x0D(cipher[9]) ^
                                gf_mult_0x09(cipher[10]) ^
                                gf_mult_0x0E(cipher[11]);


            controled_matrix[12] = gf_mult_0x0E(cipher[12]) ^  
                                gf_mult_0x0B(cipher[13]) ^
                                gf_mult_0x0D(cipher[14]) ^
                                gf_mult_0x09(cipher[15]);
                    
            controled_matrix[13] = gf_mult_0x09(cipher[12]) ^
                                gf_mult_0x0E(cipher[13]) ^
                                gf_mult_0x0B(cipher[14]) ^ 
                                gf_mult_0x0D(cipher[15]);

            controled_matrix[14] = gf_mult_0x0D(cipher[12]) ^
                                gf_mult_0x09(cipher[13]) ^  
                                gf_mult_0x0E(cipher[14]) ^
                                gf_mult_0x0B(cipher[15]);

            controled_matrix[15] = gf_mult_0x0B(cipher[12]) ^
                                gf_mult_0x0D(cipher[13]) ^
                                gf_mult_0x09(cipher[14]) ^
                                gf_mult_0x0E(cipher[15]);


        }else{
            ((WORD*)controled_matrix)[0] = (WORD)((WORD*)cipher)[0];
            ((WORD*)controled_matrix)[1] = (WORD)((WORD*)cipher)[1];
            ((WORD*)controled_matrix)[2] = (WORD)((WORD*)cipher)[2];
            ((WORD*)controled_matrix)[3] = (WORD)((WORD*)cipher)[3]; 
        }

        /*  
            [Byte Inverse Substitution Layer]
            ::Here we will use our lookup table in order to insert confusion into our algorithm
            ** Access of the confusion matrix provides the byte substitution
            [Row Shifting Layer] 
            :: byte-wise permutation of the state matrix. It is the 1th step in generating diffusion
            ** Index of the cypher provides the permutation from the shift row

            > to reduce looping overhead, the definitions will be made from byte-to-byte, this makes the assembly not have to compute at least 4 commands that do nothing for our algorithm (CMP)
                -down side is that the final program binary will be bigger
            > the row shift permutation is done automatically to save on function calling
        */
        cipher[0] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[0])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[0])];
        cipher[5] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[1])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[1])];
        cipher[10] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[2])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[2])];
        cipher[15] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[3])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[3])];

        cipher[4] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[4])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[4])];
        cipher[9] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[5])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[5])];
        cipher[14] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[6])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[6])];
        cipher[3] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[7])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[7])];

        cipher[8] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[8])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[8])];
        cipher[13] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[9])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[9])];
        cipher[2] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[10])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[10])];
        cipher[7] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[11])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[11])];

        cipher[12] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[12])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[12])];
        cipher[1] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[13])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[13])];
        cipher[6] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[14])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[14])];
        cipher[11] = coherence_matrix[MOST_SIGNIFICANT_NIBBLE(controled_matrix[15])][LEAST_SIGNIFICANT_NIBBLE(controled_matrix[15])];
    } //end rounds

    /*
            [Key Addition Layer]
            :: simple XOR with the generated extended subkey
    */
    ((WORD*)cipher)[0] ^= extended_key[0];
    ((WORD*)cipher)[1] ^= extended_key[1];
    ((WORD*)cipher)[2] ^= extended_key[2];
    ((WORD*)cipher)[3] ^= extended_key[3]; 

}