#ifdef DEBUG
    #include <math.h>
    #include <stdio.h>
double find_integral(double l, double r, double (*func)(double));
double f(double x) { return x * x; }
int main(void)
{
    double ans = find_integral(0., 2., f);
    printf("%lf\n", ans);
    return 0;
}
#endif
#include <math.h>
#define EPS 1e-3
// #define TRAPEZOIDAL_RULE
// #define RECTANGLE_RULE

#ifdef RECTANGLE_RULE
const char INTEGRAL_METHOD[] = "RECTANGLE RULE";
double find_integral(double l, double r, double (*func)(double))
{
    double len = r - l;

    int n = ceil(len * len * len / EPS / 2); // предполагается f'(x) = 1.0 в каком-то приближении

    double h = len / n;

    double sum = 0.;
    for (int i = 0; i < n; i++)
    {
        sum += func(h * i + l) * h;
    }
    return sum;
}
#elif defined(TRAPEZOIDAL_RULE)
const char INTEGRAL_METHOD[] = "TRAPEZOIDAL RULE";
double find_integral(double l, double r, double (*func)(double))
{
    double len = r - l;

    int n = ceil(sqrt(len * len * len / EPS / 12)); // предполагается f''(x) = 1.0 в каком-то приближении

    double h = len / n;

    double sum = 0.;

    sum += (func(l) + func(r)) * h / 2;
    for (int i = 1; i < n; i++)
    {
        sum += func(l + h * i) * h;
    }
    return sum;
}
#else // SIMPSON'S_RULE
const char INTEGRAL_METHOD[] = "SIMPSON'S RULE";
double find_integral(double l, double r, double (*func)(double))
{
    double len  = (r - l);
    double len2 = len * len;

    int n = ceil(pow(len2 * len2 * len / EPS / 2880.0, 0.25)); // предполагается f''''(x) = 1.0 в каком-то приближении
    n += n % 2;

    double h = len / n;

    double sum = (func(l) + func(r)) * h / 3;
    for (int i = 1; i < n; i += 2)
    {
        sum += 4 * func(l + h * i) * h / 3;
    }
    for (int i = 2; i < n; i += 2)
    {
        sum += 2 * func(l + h * i) * h / 3;
    }
    return sum;
}
#endif
