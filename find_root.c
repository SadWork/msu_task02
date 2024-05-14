#include "lib/cvector.c"
#include "lib/macro.h"

#include <math.h>

#define EPS 1e-6
#define BISECTION_METHOD

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
#elif SECANT_METHOD

#elif NEWTON_METHOD

#else // COMBINDE_METHOD

#endif
extern double f1(double);
int main()
{
    cvector x_vec;
    double *x = cvector_init(&x_vec, sizeof(double));
    find_roots(&x_vec, 0, 4, f1);
    x = cvector_free(&x_vec);
    return 0;
}