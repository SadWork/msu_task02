#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "macro.h"
typedef struct
{
    const char *name;
    double value;
} Constant;

Constant constants[] = {
    { "e",  M_E},
    {"pi", M_PI}
};
const int num_constants = sizeof(constants) / sizeof(constants[0]);

typedef struct
{
    char *name;
    char *translation;
} Uoperation;

Uoperation uoperations[] = {
    {"sin",                      "fsin"},
    {"cos",                      "fcos"},
    {"tan",           "fptan\nfstp st0"},
    {"ctg", "fptan\nfdiv st1\nfstp st1"}
};
const int num_uoperations = sizeof(uoperations) / sizeof(uoperations[0]);

typedef struct
{
    char *name;
    char *translation;
} Boperation;
Boperation boperations[] = {
    { "/",  "fdiv"},
    {"\\",  "fdiv"},
    { "+", "faddp"},
    { "*", "fmulp"},
    { "-", "fsubp"}
};
const int num_boperations = sizeof(boperations) / sizeof(boperations[0]);
#endif