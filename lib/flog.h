#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <time.h>
#include <string.h>

// DEFINE NDEBUG TO DISABLE LOGS

extern FILE* logOutf;

void flogIntern (const void* val, const char* varType, const char* varName, size_t varSize, const char* fileName, const char* funcName, size_t line);

#define flog(a)                                                                                   \
            if (logOutf == NULL){                                                                 \
                logOutf = fopen ("logs_out.htm", "w");                                            \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------<br>"                    \
                "Logging session at compiled time : %s %s<br><br>", __TIME__, __DATE__);              \
            }                                                                                     \
            flogIntern (&a, typeid (a).name (), #a, sizeof (a), __FILE__, __FUNCTION__, __LINE__)

#define flogprintf(...)                                                                           \
    {if (logOutf == NULL){                                                                        \
                logOutf = fopen ("logs_out.htm", "w");                                            \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------<br>"                    \
                "Logging session at compiled time : %s %s<br><br>", __TIME__, __DATE__);              \
    }                                                                                             \
    fprintf (logOutf, __VA_ARGS__);}

#ifdef NDEBUG
#define flog(a) ;
#define flogFileInit ;
#endif