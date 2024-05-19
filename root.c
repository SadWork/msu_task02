#include "lib/macro.h"

#include <math.h>
#include <stdio.h>
#define MAX_ITER 1000
double EPS1 = 1e-3;
// #define NEWTON_METHOD
#ifdef BISECTION_METHOD
const char METHOD_NAME[] = "BISECTION METHOD";

double find_root(double l, double r, double (*func)(double), int *itrs)
{
    *itrs        = 0;
    int cnt_itrs = 0;
    l += EPS1, r -= EPS1;
    if (r - l < EPS1)
    {
        return NAN;
    }

    double yl = func(l), yr = func(r);
    if (!(signbit(yl) ^ signbit(yr)))
    {
        return NAN; // Одинаковые знаки на концах отрезка
    }

    if (yl > yr)
    {
        double tmp = r;
        // Сохраняем инвариант yl <= 0, yr > 0
        r = l;
        l = tmp;
    }

    double m;
    while (fabs(r - l) > EPS1)
    {
        m = (l + r) / 2;
        if (func(m) > 0)
        {
            r = m;
        }
        else
        {
            l = m;
        }
        cnt_itrs++;
    }
    *itrs = cnt_itrs;

    return m;
}
#elif defined(SECANT_METHOD)
const char METHOD_NAME[] = "SECANT METHOD";

double find_root(double l, double r, double (*func)(double), int *itrs)
{
    *itrs        = 0;
    int cnt_itrs = 0;
    l += EPS1, r -= EPS1;
    if (r - l < EPS1)
    {
        return NAN;
    }

    double yl = func(l), yr = func(r);
    if (!(signbit(yl) ^ signbit(yr)))
    {
        return NAN; // Одинаковые знаки на концах отрезка
    }

    if (yl > yr)
    {
        double tmp = r;
        r          = l;
        l          = tmp;

        tmp = yr;
        yr  = yl;
        yl  = tmp;
    }
    double m = 0;
    while (fabs(r - l) > EPS1)
    {
        m         = r - (yr * (r - l) / (yr - yl));
        double ym = func(m);

        if (fabs(ym) < EPS1)
        {
            break; // Нашли корень с нужной точностью
        }

        if (signbit(ym) == signbit(yl))
        {
            l  = m;
            yl = ym;
        }
        else
        {
            r  = m;
            yr = ym;
        }
        cnt_itrs++;
    }
    *itrs = cnt_itrs;
    return m;
}
#elif defined(NEWTON_METHOD)
const char METHOD_NAME[] = "NEWTON METHOD";

double func_deriv(double x, double (*func)(double)) // error = O(EPS1^3)
{
    double f2 = func(x + EPS1), f3 = func(x - EPS1);
    double first_der = (f2 - f3) / (2 * EPS1);
    return first_der;
}
double find_root(double l, double r, double (*func)(double), int *itrs)
{
    *itrs        = 0;
    int cnt_itrs = 0;
    l += EPS1, r -= EPS1;
    if (r - l < EPS1)
    {
        return NAN;
    }
    double cur_x = r; // Начальное приближение — середина интервала

    double y = func(cur_x);
    while (fabs(y) > EPS1)
    {
        double y_prime = func_deriv(cur_x, func);
        if (fabs(y_prime) < EPS1)
        {
            return NAN;
        }
        double next_x = cur_x - y / y_prime;

        cur_x = next_x;
        y     = func(cur_x);
        cnt_itrs++;
    }
    *itrs = cnt_itrs;

    return cur_x;
}
#else // COMBINED_METHOD
const char METHOD_NAME[] = "COMBINED METHOD";

double func_second_derivative(double x, double (*func)(double)) // error = O(EPS1^4)
{
    double f1 = func(x), f2 = func(x + EPS1), f3 = func(x - EPS1);
    double second_der = (f2 + f3 - 2 * f1) / EPS1 / EPS1;
    return second_der;
}
double func_deriv(double x, double (*func)(double)) // error = O(EPS1^3)
{
    double f2 = func(x + EPS1), f3 = func(x - EPS1);
    double first_der = (f2 - f3) / (2 * EPS1);
    return first_der;
}

double find_root(double l, double r, double (*func)(double), int *itrs)
{
    *itrs        = 0;
    int cnt_itrs = 0;

    l += EPS1, r -= EPS1;
    if (r - l < EPS1)
    {
        return NAN;
    }

    double y = func((r + l) / 2);
    while (fabs(y) > EPS1)
    {
        double yl = func(l), yr = func(r);

        double y_prime        = func_deriv((l + r) / 2, func);
        double y_double_prime = func_second_derivative((l + r) / 2, func);

        if (fabs(y_prime) < EPS1)
        {
            return NAN;
        }
        else if (signbit(y_prime) == signbit(y_double_prime))
        {
            y_prime         = func_deriv(r, func);
            double m_secant = l - (yl * (r - l) / (yr - yl));
            double m_newton = r - yr / y_prime;
            l = m_secant, r = m_newton;
        }
        else
        {
            y_prime         = func_deriv(l, func);
            double m_secant = r - (yr * (r - l) / (yr - yl));
            double m_newton = l - yl / y_prime;
            r = m_secant, l = m_newton;
        }
        y = func((l + r) / 2);
        cnt_itrs++;
    }
    *itrs = cnt_itrs;
    return (l + r) / 2;
}

#endif

#ifdef DEBUG
double f1(double x) { return cos(x); }

int main()
{
    int iters  = 0;
    double ans = find_root(1, 4, f1, &iters);
    printf("%.12lf %i\n", ans, iters);
    return 0;
}
#endif