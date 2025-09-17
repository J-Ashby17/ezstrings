#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

void context_search(const char *filename, const char *target, int context_bytes){
    FILE *file = fopen(filename, "rb");
    if (!file){
        perror("fopen");
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    // Allocate buffer
    unsigned char *data = (unsigned char*)malloc(filesize);
    if (!data){
        perror("malloc");
        fclose(file);
        return;
    }

    fread(data, 1, filesize, file);
    fclose(file);

    size_t tlen = strlen(target);
    if (tlen == 0 || tlen > filesize) {
        free(data);
        return;
    }

    for (size_t i = 0; i + tlen <= filesize; ++i){
        if (memcmp(data + i, target, tlen) == 0){
            size_t start;
            if (i > (size_t)context_bytes) {
                start = i - context_bytes;
            } else {
                start = 0;
            }

            size_t end;
            if (i + tlen + context_bytes < filesize) {
                end = i + tlen + context_bytes;
            } else {
                end = filesize;
            }

            printf("Offset match at 0x%zx:\n", i);
            for (size_t j = start; j < end; ++j){
                unsigned char c = data[j];
                if (c >= 32 && c <= 126) {
                    putchar(c);
                } else {
                    putchar('.');
                }
            }
            printf("\n\n");
        }
    }

    free(data);
}
