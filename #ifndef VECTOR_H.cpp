#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double *data;
    int size;
} Vector;

void vector_init(Vector *v, int size);
void vector_free(Vector *v);
void vector_input(Vector *v);
double vector_sum(Vector *v);
double vector_mean(Vector *v);
double vector_variance(Vector *v);
double vector_stddev(Vector *v);
double vector_min(Vector *v);
double vector_max(Vector *v);
double vector_median(Vector *v);
double vector_mode(Vector *v);
void vector_quartiles(Vector *v, double *q1, double *q2, double *q3);
void print_stats(Vector *v);
void print_adv_stats(Vector *v);

#endif
