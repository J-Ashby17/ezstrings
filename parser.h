#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#define MIN_LEN 4
#define MAX_FILENAME 512
#define MAX_STRINGS 2048
#define MAX_STRING_LENGTH 1024

/* simple parsers (optional to keep) */
void parse_utf16le(const char *filename);
void parse_utf16be(const char *filename);
void parse_ascii(const char *filename);

/* two-pass de-dupers */
void nodupes_utf16(const char *filename);     /* handles LE + BE together */
void nodupes_ascii_utf8(const char *filename);

#endif
