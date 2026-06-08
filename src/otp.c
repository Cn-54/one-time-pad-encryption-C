#include <string.h>
#include "encryption.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage:\n");
        printf("  otp encrypt <file>\n");
        printf("  otp decrypt <file> <key>\n");
        return 1;
    }

    if (strcmp(argv[1], "encrypt") == 0) {

        if (argc != 3) {
            printf("Usage: otp encrypt <file>\n");
            return 1;
        }

        encrypt(argv[2]);
    }
    else if (strcmp(argv[1], "decrypt") == 0) {

        if (argc != 4) {
            printf("Usage: otp decrypt <file> <key>\n");
            return 1;
        }

        decrypt(argv[2], argv[3]);
    }
    else {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}