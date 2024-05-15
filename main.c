#include "root.h"
#include "segment.h"

#include <stdio.h>
extern double f1(double);
extern double f2(double);
extern double f3(double);

double g12(double x) { return f1(x) - f2(x); }
double g13(double x) { return f1(x) - f3(x); }
double g23(double x) { return f2(x) - f3(x); }

int main(void)
{
    int itrs = 0;
    printf("g12: %lf %lf %lf %i\n", find_root(LEFT_EDGE, RIGHT_EDGE, g12, &itrs), LEFT_EDGE, RIGHT_EDGE, itrs);
    printf("g13: %lf %lf %lf %i\n", find_root(LEFT_EDGE, RIGHT_EDGE, g13, &itrs), LEFT_EDGE, RIGHT_EDGE, itrs);
    printf("g23: %lf %lf %lf %i\n", find_root(LEFT_EDGE, RIGHT_EDGE, g23, &itrs), LEFT_EDGE, RIGHT_EDGE, itrs);

    return 0;
}