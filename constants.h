#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "macro.h"

#ifndef M_E
    #define M_E 2.7182818284590452354
#endif
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct
{
    const char *name;
    double value;
} Constant;

typedef struct
{
    char *name;
    char *translation;
} Boperation;

typedef struct
{
    char *name;
    char *translation;
} Uoperation;

Constant constants[] = {
    { "e",  M_E},
    {"pi", M_PI}
};

Uoperation uoperations[] = {
    {"sin",                      "fsin"},
    {"cos",                      "fcos"},
    {"tan",           "fptan\nfstp st0"},
    {"ctg", "fptan\nfdiv st1\nfstp st1"}
};

Boperation boperations[] = {
    { "/",  "fdiv"},
    {"\\",  "fdiv"},
    { "+", "faddp"},
    { "*", "fmulp"},
    { "-", "fsubp"}
};

const int num_constants   = sizeof(constants) / sizeof(constants[0]);
const int num_boperations = sizeof(boperations) / sizeof(boperations[0]);
const int num_uoperations = sizeof(uoperations) / sizeof(uoperations[0]);
#endif