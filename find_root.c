#include "lib/cvector.c"
#include "lib/macro.h"

#include <math.h>

#define EPS      1e-6
#define MAX_ITER 1000
#define COMBINED_METHOD

#ifdef BISECTION_METHOD
double find_root(double l, double r, double (*func)(double))
{
    l += EPS, r -= EPS;
    if (r - l < EPS)
    {
        return NAN;
    }

    double yl = func(l), yr = func(r);
    if (!(signbit(yl) ^ signbit(r)))
    {
        return NAN; // Одинаковые знаки на концах отрезка
    }

    if (yl > yr)
    {
        double tmp = yr;
        // Сохраняем инвариант yl <= 0, yr > 0
        yr = yl;
        yl = tmp;
    }

    double m;
    while (r - l > EPS)
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
    }
    return m;
}
#elif defined(SECANT_METHOD)
double find_root(double l, double r, double (*func)(double))
{
    l += EPS, r -= EPS;
    if (r - l < EPS)
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
        double tmp = yr;
        yr         = yl;
        yl         = tmp;
    }
    double m;
    while (fabs(r - l) > EPS)
    {
        m         = r - (yr * (r - l) / (yr - yl));
        double ym = func(m);

        if (fabs(ym) < EPS)
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
    }

    return m;
}
#elif defined(NEWTON_METHOD)
double func_deriv(double x, double (*func)(double)) // error = O(eps^3)
{
    double f2 = func(x + EPS), f3 = func(x - EPS);
    double first_der = (f2 - f3) / (2 * EPS);
    return first_der;
}
double find_root(double l, double r, double (*func)(double))
{
    l += EPS, r -= EPS;
    if (r - l < EPS)
    {
        return NAN;
    }
    double cur_x = r; // Начальное приближение — середина интервала

    double y = func(cur_x);
    while (fabs(y) > EPS)
    {
        double y_prime = func_deriv(cur_x, func);
        if (fabs(y_prime) < EPS)
        {
            return NAN;
        }
        double next_x = cur_x - y / y_prime;

        cur_x = next_x;
        y     = func(cur_x);
    }
    return cur_x;
}
#else // COMBINDE_METHOD
double func_second_derivative(double x, double (*func)(double)) // error = O(eps^4)
{
    double f1 = func(x), f2 = func(x + EPS), f3 = func(x - EPS);
    double second_der = (f2 + f3 - 2 * f1) / EPS / EPS;
    return second_der;
}
double func_deriv(double x, double (*func)(double)) // error = O(eps^3)
{
    double f2 = func(x + EPS), f3 = func(x - EPS);
    double first_der = (f2 - f3) / (2 * EPS);
    return first_der;
}

double find_root(double l, double r, double (*func)(double))
{
    l += EPS, r -= EPS;
    if (r - l < EPS)
    {
        return NAN;
    }

    double y = func((r + l) / 2);
    while (fabs(y) > EPS)
    {
        double yl = func(l), yr = func(r);

        double y_prime        = func_deriv((l + r) / 2, func);
        double y_double_prime = func_second_derivative((l + r) / 2, func);

        if (fabs(y_prime) < EPS)
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
    }
    return (l + r) / 2;
}

#endif
double f1(double x) { return x * x - 2; };
int main()
{
    printf("%.12lf\n", find_root(-4, 4, f1));
    return 0;
}