#ifndef MATH_UTILS_H
#define MATH_UTILS_H

double factorial(int n);
double permutation(int n, int r);
double combination(int n, int r);
int gcd(int a, int b);
int lcm(int a, int b);
double eval_expr(const char *expr, double x);
double derivative(const char *expr, double x);
double integral(const char *expr, double a, double b);
void plot_function(const char *expr, double minX, double maxX, double minY, double maxY);

#endif
