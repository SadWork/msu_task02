#include "integral.h"
#include "root.h"
#include "segment.h"

#include <stdio.h>
#include <string.h>
extern double f1(double);
extern double f2(double);
extern double f3(double);

double g12(double x) { return f1(x) - f2(x); }
double g13(double x) { return f1(x) - f3(x); }
double g23(double x) { return f2(x) - f3(x); }

void print_help()
{
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf("  -a    Print the abscissas of the intersection points of the curves\n");
    printf("  -i    Print the number of iterations required for the approximate solution of the equations when searching for intersection points\n");
    printf("  -m    Print the method used to find intersection\n");
    printf("  --help    Display this help message\n");
}

int main(int argc, char *argv[])
{
    int print_abscissas  = 0;
    int print_iterations = 0;
    int print_method     = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            print_abscissas = 1;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            print_iterations = 1;
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            print_method = 1;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            print_help();
            return 0;
        }
        else
        {
            printf("Unknown option: %s\n", argv[i]);
            print_help();
            return 1;
        }
    }
    if (print_method)
    {
        printf("%s is used now to find intersections\n", METHOD_NAME);
        printf("%s is used now to find integrals\n", INTEGRAL_METHOD);
    }

    int itrs      = 0;
    double root12 = find_root(LEFT_EDGE, RIGHT_EDGE, g12, &itrs);
    int itrs12    = itrs;
    double root13 = find_root(LEFT_EDGE, RIGHT_EDGE, g13, &itrs);
    int itrs13    = itrs;
    double root23 = find_root(LEFT_EDGE, RIGHT_EDGE, g23, &itrs);
    int itrs23    = itrs;

    if (print_abscissas)
    {
        printf("f1 and f2 intersect in x = %lf\n", root12);
        printf("f1 and f3 intersect in x = %lf\n", root13);
        printf("f2 and f3 intersect in x = %lf\n", root23);
    }
    if (print_iterations)
    {
        printf("to find f1 and f2 intersectoin spent iterations: %d\n", itrs12);
        printf("to find f1 and f3 intersectoin spent iterations: %d\n", itrs13);
        printf("to find f2 and f3 intersectoin spent iterations: %d\n", itrs23);
    }

    return 0;
}