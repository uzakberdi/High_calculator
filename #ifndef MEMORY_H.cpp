#ifndef MEMORY_H
#define MEMORY_H

#define MAX_MEMORY_SIZE 5

typedef struct {
    double data[MAX_MEMORY_SIZE];
    int size;
} Memory;

void memory_init(Memory *mem);
void memory_store(Memory *mem, double result);
double memory_recall(Memory *mem, int index);
void memory_show(Memory *mem);

#endif
