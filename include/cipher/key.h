#ifndef _AES_KEY_H_
#define _AES_KEY_H_

#include "types.h"
void key_expansion(WORD key[8], WORD extended_key[60]);
extern const BYTE confusion_matrix[16][16];

#endif //_AES_KEY_H_