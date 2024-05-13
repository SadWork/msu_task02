#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "macro.h"

#include <limits.h>

#ifndef M_E
    #define M_E 2.7182818284590452354
#endif
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#define X87_REGISTERS_COUNT 8
typedef struct
{
    const char *name;
    double value;
} Constant;

typedef struct
{
    char *name;
    char *translation;
    char is_commutative;
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
    {"sin",                          "fsin"},
    {"cos",                          "fcos"},
    {"tan",             "fptan\n\tfstp st0"},
    {"ctg", "fptan\n\tfdiv st1\n\tfstp st1"}
};

Boperation boperations[] = {
    {  "/", "fdivp",              0},
    { "\\", "fdivp",              0},
    {  "+", "faddp", IS_COMMUTATIVE},
    {  "*", "fmulp", IS_COMMUTATIVE},
    {  "-", "fsubp",              0},
    {"–", "fsubp",              0}
};

const int num_constants   = sizeof(constants) / sizeof(constants[0]);
const int num_boperations = sizeof(boperations) / sizeof(boperations[0]);
const int num_uoperations = sizeof(uoperations) / sizeof(uoperations[0]);
#endif