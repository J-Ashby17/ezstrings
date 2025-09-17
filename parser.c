#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

// helper functions

static void add_if_unique(char *arr[], int *count, const char *s) {
    for (int i = 0; i < *count; ++i) {
        if (strcmp(arr[i], s) == 0) return;
    }
    if (*count < MAX_STRINGS) {
        arr[*count] = strdup(s);
        if (arr[*count]) (*count)++;
    }
}

static void print_and_free_all(char *arr[], int count, const char *label) {
    for (int i = 0; i < count; ++i) {
        if (label && *label) printf("%s %s\n", label, arr[i]);
        else                 printf("%s\n", arr[i]);
        free(arr[i]);
    }
}

void parse_utf16le(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { perror("fopen"); return; }

    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    unsigned char *buf = (unsigned char*)malloc(sz);
    if (!buf) { perror("malloc"); fclose(f); return; }
    fread(buf, 1, sz, f);
    fclose(f);

    char run[1024] = {0};
    int ri = 0;

    for (size_t i = 0; i + 1 < sz; ) {
        unsigned char a = buf[i], b = buf[i+1];
        if (isprint(a) && b == 0x00) {
            if (ri < (int)sizeof(run) - 1) run[ri++] = (char)a;
            i += 2;
        } else {
            if (ri >= MIN_LEN) { run[ri] = '\0'; printf("UTF16LE %s\n", run); }
            ri = 0;
            i += 1;
        }
    }
    if (ri >= MIN_LEN) { run[ri] = '\0'; printf("UTF16LE %s\n", run); }

    free(buf);
}

void parse_utf16be(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { perror("fopen"); return; }

    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    unsigned char *buf = (unsigned char*)malloc(sz);
    if (!buf) { perror("malloc"); fclose(f); return; }
    fread(buf, 1, sz, f);
    fclose(f);

    char run[1024] = {0};
    int ri = 0;

    for (size_t i = 0; i + 1 < sz; ) {
        unsigned char a = buf[i], b = buf[i+1];
        if (a == 0x00 && isprint(b)) {
            if (ri < (int)sizeof(run) - 1) run[ri++] = (char)b;
            i += 2;
        } else {
            if (ri >= MIN_LEN) { run[ri] = '\0'; printf("UTF16BE %s\n", run); }
            ri = 0;
            i += 1;
        }
    }
    if (ri >= MIN_LEN) { run[ri] = '\0'; printf("UTF16BE %s\n", run); }

    free(buf);
}

void parse_ascii(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) { perror("fopen"); return; }

    char buffer[1024]; size_t idx = 0; int b;
    while ((b = fgetc(file)) != EOF) {
        if (isprint(b) || b=='\t') {
            if (idx < sizeof(buffer)-1) buffer[idx++] = (char)b;
        } else {
            if (idx >= MIN_LEN) { buffer[idx]='\0'; printf("ASCII/UTF-8 %s\n", buffer); }
            idx = 0;
        }
    }
    if (idx >= MIN_LEN) { buffer[idx]='\0'; printf("ASCII/UTF-8 %s\n", buffer); }
    fclose(file);
}

// UTF16(BE +LE) no dupes

void nodupes_utf16(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { perror("fopen"); return; }

    
    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    unsigned char *buf = (unsigned char*)malloc(sz);
    if (!buf) { perror("malloc"); fclose(f); return; }
    fread(buf, 1, sz, f);
    fclose(f);

    char *uniq[MAX_STRINGS]; int nuniq = 0;
    char le[MAX_STRING_LENGTH] = {0}, be[MAX_STRING_LENGTH] = {0};
    int li = 0, bi = 0;

    for (size_t i = 0; i + 1 < sz; ++i) {
        unsigned char a = buf[i], b = buf[i+1];

        // LE: char, 0x00
        if (isprint(a) && b == 0x00) {
            if (li < MAX_STRING_LENGTH-1) le[li++] = (char)a;
            i++; // get rid of 0x00
        } else {
            if (li >= MIN_LEN) { le[li] = '\0'; add_if_unique(uniq, &nuniq, le); }
            li = 0;
        }

        // BE: 0x00, char
        if (a == 0x00 && isprint(b)) {
            if (bi < MAX_STRING_LENGTH-1) be[bi++] = (char)b;
            i++; 
        } else {
            if (bi >= MIN_LEN) { be[bi] = '\0'; add_if_unique(uniq, &nuniq, be); }
            bi = 0;
        }
    }

    if (li >= MIN_LEN) { le[li] = '\0'; add_if_unique(uniq, &nuniq, le); }
    if (bi >= MIN_LEN) { be[bi] = '\0'; add_if_unique(uniq, &nuniq, be); }

    print_and_free_all(uniq, nuniq, "UTF16");

    free(buf);
}

// ASCII/UTF8(1byte)

void nodupes_ascii_utf8(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { perror("fopen"); return; }

    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    unsigned char *buf = (unsigned char*)malloc(sz);
    if (!buf) { perror("malloc"); fclose(f); return; }
    fread(buf, 1, sz, f);
    fclose(f);

    char *uniq[MAX_STRINGS]; int nuniq = 0;
    char run[MAX_STRING_LENGTH] = {0};
    int ri = 0;

    for (size_t i = 0; i < sz; ++i) {
        int b = buf[i];
        if (isprint(b) || b=='\t') {
            if (ri < MAX_STRING_LENGTH-1) run[ri++] = (char)b;
        } else {
            if (ri >= MIN_LEN) { run[ri]='\0'; add_if_unique(uniq, &nuniq, run); }
            ri = 0;
        }
    }
    if (ri >= MIN_LEN) { run[ri]='\0'; add_if_unique(uniq, &nuniq, run); }

    print_and_free_all(uniq, nuniq, "ASCII/UTF-8");

    free(buf);
}
