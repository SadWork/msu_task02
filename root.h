#ifndef ROOT_H
#define ROOT_H
double find_root(double l, double r, double (*func)(double), int *itrs);
extern const char METHOD_NAME[];
#endif