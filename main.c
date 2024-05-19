#include "integral.h"
#include "root.h"
#include "segment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-3

extern double f1(double);
extern double f2(double);
extern double f3(double);

double g12(double x) { return f1(x) - f2(x); }
double g13(double x) { return f1(x) - f3(x); }
double g23(double x) { return f2(x) - f3(x); }

typedef struct
{
    double x;
    double y;
    double (*func)(double);
} FuncAndPoint;

int compareFuncAndPoint(const void *a, const void *b)
{
    FuncAndPoint *fa = (FuncAndPoint *)a;
    FuncAndPoint *fb = (FuncAndPoint *)b;

    if (fa->x < fb->x)
    {
        return -1;
    }
    else if (fa->x > fb->x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void print_help()
{
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf("  -a    Print the abscissas of the intersection points of the curves\n");
    printf("  -o    Print the ordinates of the intersection points of the curves\n");
    printf("  -i    Print the number of iterations required for the approximate solution of the equations when searching for intersection points\n");
    printf("  -m    Print the method used to find intersection\n");
    printf("  --help    Display this help message\n");
}

int main(int argc, char *argv[])
{
    int print_abscissas  = 0;
    int print_ordinates  = 0;
    int print_iterations = 0;
    int print_method     = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            print_abscissas = 1;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            print_ordinates = 1;
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

    EPS1 = EPS / 2;
    FuncAndPoint func_arr[3];

    int itrs      = 0;
    double root12 = find_root(LEFT_EDGE, RIGHT_EDGE, g12, &itrs);
    func_arr[0].x = root12, func_arr[0].y = f1(root12), func_arr[0].func = g12;
    int itrs12 = itrs;

    double root13 = find_root(LEFT_EDGE, RIGHT_EDGE, g13, &itrs);
    func_arr[1].x = root13, func_arr[1].y = f3(root13), func_arr[1].func = g13;
    int itrs13 = itrs;

    double root23 = find_root(LEFT_EDGE, RIGHT_EDGE, g23, &itrs);
    func_arr[2].x = root23, func_arr[2].y = f2(root23), func_arr[2].func = g23;
    int itrs23 = itrs;

    if (print_abscissas)
    {
        printf("f1 and f2 intersect in x = %lf\n", root12);
        printf("f1 and f3 intersect in x = %lf\n", root13);
        printf("f2 and f3 intersect in x = %lf\n", root23);
        printf("\n");
    }
    if (print_ordinates)
    {
        printf("f1 and f2 intersect in y = %lf\n", func_arr[0].y);
        printf("f1 and f3 intersect in y = %lf\n", func_arr[1].y);
        printf("f2 and f3 intersect in y = %lf\n", func_arr[2].y);
        printf("\n");
    }
    if (print_iterations)
    {
        printf("to find f1 and f2 intersectoin spent iterations: %d\n", itrs12);
        printf("to find f1 and f3 intersectoin spent iterations: %d\n", itrs13);
        printf("to find f2 and f3 intersectoin spent iterations: %d\n", itrs23);
        printf("\n");
    }

    const int size_func_arr = sizeof(func_arr) / sizeof(func_arr[0]);
    qsort(func_arr, size_func_arr, sizeof(func_arr[0]), compareFuncAndPoint);

    {
        // Вычисляем значение погрешности при вычислении интеграла
        double tmp = func_arr[0].y + func_arr[size_func_arr - 1].y;
        for (int i = 1; i < size_func_arr - 1; ++i)
        {
            tmp += 2 * func_arr[i].y;
        }
        EPS2 = EPS / tmp;
    }

    double S = fabs(find_integral(func_arr[0].x, func_arr[1].x, func_arr[0].func));
    S += fabs(find_integral(func_arr[1].x, func_arr[2].x, func_arr[2].func));

    printf("%lf\n", S);
    return 0;
}