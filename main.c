#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "search.h"

#define EZSTRINGS_VERSION "1.0"

// Print a simple banner (program name + version)
static void print_banner(void) {
    printf("============================================\n");
    printf(" ezstrings v%s - Binary String Extractor\n", EZSTRINGS_VERSION);
    printf("============================================\n\n");
}

// Print usage/help menu
static void print_usage(const char *progname) {
    printf("Usage: %s --file <filename> [OPTIONS]\n", progname);
    printf("\nOptions:\n");
    printf("  --parse              Parse file as UTF-16LE, UTF-16BE, and ASCII/UTF-8\n");
    printf("  --nodupes            Parse and show only unique strings\n");
    printf("  --search <term>      Search for a term in the file\n");
    printf("  --context N          Show N bytes of context around matches (with --search)\n");
    printf("  --help, -h           Show this help menu\n");
    printf("  --version, -v        Show program version only\n");
    printf("\nExamples:\n");
    printf("  %s --file test.bin --parse\n", progname);
    printf("  %s --file test.bin --nodupes\n", progname);
    printf("  %s --file test.bin --search admin --context 16\n", progname);
    printf("\n");
}

int main(int argc, char *argv[]) {
    char *filename = NULL;    // file to operate on
    char *search_term = NULL; // term to search for
    int context_bytes = 0;    // number of context bytes around matches
    int parse_flag = 0;       // run --parse
    int nodupe_flag = 0;      // run --nodupes
    int search_flag = 0;      // run --search
    int help_flag = 0;        // run --help
    int version_flag = 0;     // run --version

    // Parse command line arguments
    for (int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "--parse") == 0) {
            parse_flag = 1;
        } else if (strcmp(argv[i], "--search") == 0 && i + 1 < argc) {
            search_term = argv[++i];
            search_flag = 1;
        } else if (strcmp(argv[i], "--context") == 0 && i + 1 < argc) {
            context_bytes = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--nodupes") == 0) {
            nodupe_flag = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help_flag = 1;
        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            version_flag = 1;
        }
    }

    // --version
    if (version_flag) {
        printf("ezstrings v%s\n", EZSTRINGS_VERSION);
        return 0;
    }

    // starting load banner
    print_banner();

    if (help_flag || !filename) {
        print_usage(argv[0]);
        return 0;   // not an error
    }

    // parse all encodings
    if (parse_flag){
        printf("> Parsing all encodings:\n");
        parse_utf16le(filename);
        parse_utf16be(filename);
        parse_ascii(filename);
    }

    // no duplicate results (two passes sad emoji)
    if (nodupe_flag){
        printf("> Pass 1 (UTF-16 LE/BE) no duplicates:\n");
        nodupes_utf16(filename);
        printf("\n> Pass 2 (ASCII/UTF-8) no duplicates:\n");
        nodupes_ascii_utf8(filename);
    }

    // search can be used in combo with context(hopefully yall can use this for ctfs)
    if (search_flag){
        printf("> Searching for \"%s\" with context %d:\n", search_term, context_bytes);
        context_search(filename, search_term, context_bytes);
    }

    return 0;
}
