#include "encryption.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>


void xor_crypt(uint8_t data[], uint8_t key[],size_t len){
    for(size_t i = 0; i < len; i++){
        data[i] = data[i] ^ key[i];
    }
}

void generateKey(uint8_t key[],size_t len){
    getrandom(key, len, 0);
}

size_t getFileSize(FILE *file){
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    return size;
}

void fileToBuffer(FILE *file,uint8_t buffer[],size_t size){
    fread(buffer, 1, size, file);
}

void bufferToFile(FILE *file,uint8_t buffer[],size_t size){
    fwrite(buffer, 1, size, file);
}

void removeExt(const char *input, char *output, size_t out_size)
{
    strncpy(output, input, out_size);
    output[out_size - 1] = '\0';

    char *dot = strrchr(output, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

void encrypt(const char *filename) {
    FILE *in = fopen(filename, "rb");
    if (!in) return;

    size_t size = getFileSize(in);

    uint8_t *data = malloc(size);
    uint8_t *key  = malloc(size);

    if (!data || !key) {
        free(data);
        free(key);
        fclose(in);
        return;
    }

    fileToBuffer(in, data, size);
    fclose(in);

    generateKey(key, size);
    xor_crypt(data, key, size);

    char outName[256];
    char keyName[256];

    snprintf(outName, sizeof(outName), "%s.enc", filename);
    snprintf(keyName, sizeof(keyName), "%s.key", filename);

    FILE *out     = fopen(outName, "wb");
    FILE *keyFile = fopen(keyName, "wb");

    if (!out || !keyFile) {
        free(data);
        free(key);
        return;
    }

    bufferToFile(out, data, size);
    bufferToFile(keyFile, key, size);

    fclose(out);
    fclose(keyFile);
    free(data);
    free(key);
}

void decrypt(const char *inFileName, const char *keyFileName) {
    FILE *inFile = fopen(inFileName, "rb");
    if (!inFile) return;

    FILE *keyFile = fopen(keyFileName, "rb");
    if (!keyFile) {
        fclose(inFile);
        return;
    }

    size_t size   = getFileSize(inFile);
    uint8_t *data = malloc(size);
    uint8_t *key  = malloc(size);

    if (!data || !key) {
        free(data);
        free(key);
        fclose(inFile);
        fclose(keyFile);
        return;
    }

    fileToBuffer(inFile, data, size);
    fileToBuffer(keyFile, key, size);

    fclose(inFile);
    fclose(keyFile);

    xor_crypt(data, key, size);

    char outName[256];
    removeExt(inFileName, outName, sizeof(outName));

    FILE *out = fopen(outName, "wb");
    if (!out) {
        free(data);
        free(key);
        return;
    }

    bufferToFile(out, data, size);

    fclose(out);
    free(data);
    free(key);
}