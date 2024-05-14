#include "lib/cvector.c"
#include "lib/macro.h"

#include <math.h>

#define EPS 1e-6
#define SECANT_METHOD

#ifdef BISECTION_METHOD
void find_roots(cvector *x, double l, double r, double (*func)(double))
{
    double cur_l = l + EPS, cur_r = r - EPS;
    if (cur_r - cur_l < EPS)
    {
        return;
    }

    double yl = func(cur_l), yr = func(cur_r);
    if (!(signbit(yl) ^ signbit(r)))
    {
        return; // Одинаковые знаки на концах отрезка
    }

    if (yl > yr)
    {
        double tmp = yr;
        // Сохраняем инвариант yl <= 0, yr > 0
        yr = yl;
        yl = tmp;
    }

    double m;
    while (cur_r - cur_l > EPS)
    {
        m = (cur_l + cur_r) / 2;
        if (func(m) > 0)
        {
            cur_r = m;
        }
        else
        {
            cur_l = m;
        }
    }
    find_roots(x, l + EPS, m - EPS, func);
    cvector_push_back(x, &m);
    find_roots(x, m + EPS, r - EPS, func);
}
#elif defined(SECANT_METHOD)
void find_roots(cvector *x, double l, double r, double (*func)(double))
{
    double cur_l = l + EPS, cur_r = r - EPS;
    if (cur_r - cur_l < EPS)
    {
        return;
    }

    double yl = func(cur_l), yr = func(cur_r);
    if (!(signbit(yl) ^ signbit(yr)))
    {
        return; // Одинаковые знаки на концах отрезка
    }

    if (yl > yr)
    {
        double tmp = yr;
        yr         = yl;
        yl         = tmp;
    }

    double m;
    while (fabs(cur_r - cur_l) > EPS)
    {
        m         = cur_r - (yr * (cur_r - cur_l) / (yr - yl));
        double ym = func(m);

        if (fabs(ym) < EPS)
        {
            break; // Нашли корень с нужной точностью
        }

        if (signbit(ym) == signbit(yl))
        {
            cur_l = m;
            yl    = ym;
        }
        else
        {
            cur_r = m;
            yr    = ym;
        }
    }

    find_roots(x, l + EPS, m - EPS, func);
    cvector_push_back(x, &m);
    find_roots(x, m + EPS, r - EPS, func);
}
#elif defined(NEWTON_METHOD)

#else // COMBINDE_METHOD

#endif
double f1(double x) { return x + 2; };
int main()
{
    cvector x_vec;
    double *x = cvector_init(&x_vec, sizeof(double));
    find_roots(&x_vec, -4, 4, f1);
    x = cvector_free(&x_vec);
    return 0;
}