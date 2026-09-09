#pragma once
#ifndef  __STDIO__
#define __STDIO__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"

#include "stdarg.h"
__CRT_START
struct __CRT_FILE_IMPL;
typedef struct __CRT_FILE{ struct __CRT_FILE_IMPL* __placeholder; } FILE;
typedef __int64 fpos_t;
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2
#define  BUFSIZ  512
#define EOF (-1)
#define FOPEN_MAX 1
#define FILENAME_MAX 260
#define _PRINTF_NAN_LEN_MAX (14) /* sign + "NAN" + "(" + signaling char + " " + up to 6 chars from low 48 payload bits + ")" */


#define L_tmpnam (64)

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

extern FILE* __stdin;
extern FILE* __stdout;
extern FILE* __stderr;

#define stdin  __stdin
#define stdout __stdout
#define stderr __stderr


int remove(const char* filename);
int rename(const char* old, const char* new);
FILE* tmpfile(void);
char* tmpnam(char* s);
int fclose(FILE* stream);
int fflush(FILE* stream);
FILE* fopen(const char* restrict filename, const char* restrict mode);
FILE* freopen(const char* restrict filename, const char* restrict mode,	FILE* restrict stream);
void setbuf(FILE* restrict stream, char* restrict buf);
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size);

int fprintf(FILE* restrict stream, const char* restrict format, ...); //this will be a pain to implement //TODO : fml
int fscanf(FILE* restrict stream, const char* restrict format, ...);
int printf(const char* restrict format, ...);
int scanf(const char* restrict format, ...);
int snprintf(char* restrict s, size_t n, const char* restrict format, ...);
int sprintf(char* restrict s, const char* restrict format, ...);
int sscanf(const char* restrict s, const char* restrict format, ...);
int vfprintf(FILE* restrict stream, const char* restrict format, va_list arg);
__CRT_END
#endif // ! __STDIO__
