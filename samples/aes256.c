#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher/encryption.h"
#include "cipher/decryption.h"

void print_help() {
    printf("Usage: aes256 -e|-d <var> <key>\n");
    printf("Options:\n");
    printf("  -e\tEncrypt a block of AES 256\n");
    printf("  -d\tDecrypt a block of AES 256\n");
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Error: Invalid number of arguments\n");
        print_help();
        return 1;
    }

    BYTE var[16];
    BYTE key[32];

    if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-d") == 0) {
        // Parse var
        for (int i = 0; i < 16; i++) {
            sscanf(argv[2] + 2 * i, "%2hhx", &var[i]);
        }

        // Parse key
        for (int i = 0; i < 32; i++) {
            sscanf(argv[3] + 2 * i, "%2hhx", &key[i]);
        }

        // Perform encryption or decryption based on the option
        if (strcmp(argv[1], "-e") == 0) {
            encrypt(var, key);
            printf("ENCRYPTED:::: ");
            for (int k = 0; k < 16; k++) {
                printf("%x", var[k]);
            }
            printf("\n");
        } else {
            decrypt(var, key);
            printf("DECRYPTED:::: ");
            for (int k = 0; k < 16; k++) {
                printf("%x", var[k]);
            }
            printf("\n");
        }
    } else {
        printf("Error: Invalid option\n");
        print_help();
        return 1;
    }

    return 0;
}
