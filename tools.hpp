#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>

typedef size_t error_t; 

#define MY_ASSERT_SOFT( cond, error, ... )                                                                           \
    {                                                                                                                \
    if (!(cond))                                                                                                     \
        {                                                                                                            \
        printf ("\nERROR %d in %s in line %d in function %s\n", error, __FILE__, __LINE__, __PRETTY_FUNCTION__);     \
        __VA_ARGS__;                                                                                                 \
        return error;                                                                                                \
        }                                                                                                            \
    }

#define MY_ASSERT_HARD( cond, ... )                                                                                  \
    {                                                                                                                \
    if (!(cond))                                                                                                     \
        {                                                                                                            \
        printf ("\nERROR in %s in line %d in function %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);               \
        __VA_ARGS__;                                                                                                 \
        abort();                                                                                                     \
        }                                                                                                            \
    }


#define max(x, y)                                                                                                   \
({                                                                                                                  \
    __typeof__(x) _tx = x; __typeof__(y) _ty = y;                                                                   \
    _tx > _ty ? _tx : _ty;                                                                                          \
})


#define min(x, y)                                                                                                   \
({                                                                                                                  \
    __typeof__(x) _tx = x; __typeof__(y) _ty = y;                                                                   \
    _tx < _ty ? _tx : _ty;                                                                                          \
})


#define ISERROR(...)                    \
    {                                   \
    error_t error = __VA_ARGS__;        \
    if (error != 0)                     \
        return error;                   \
    }

struct String
{
    char* linePtr;
    size_t length;
};

struct Text
{
    char* bufPtr;
    String* lines;
    size_t size;
    size_t nLines;
    FILE* file;
};

const double AT = 1e-6;

size_t countLines(const char* str);
size_t fileLen(const char * file);
bool isZero(double x);
bool areEqual(double a, double b);

Text setbuf(char filename_i[]);
String* setPtr(char* buf, size_t nLines, size_t flen);
void printBuf(String* pointers);
void bufReturn(char* buf, size_t flen);
void bufClear(void);

FILE *fileopener(char filename[]);
FILE *fileopenerRB(char filename[]);
FILE *fileopenerW(char filename[]);
FILE *fileopenerWB(char filename[]);
size_t fileLen(const char * file);


#endif
