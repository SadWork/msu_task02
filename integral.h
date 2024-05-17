#ifndef INTEGRAL_H
#define INTEGRAL_H
#include <math.h>
double find_integral(double l, double r, double (*func)(double));
extern const char INTEGRAL_METHOD[];
#endif