#ifndef CVECTOR_H
#define CVECTOR_H
#include "macro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    void *data;
    int size;
    int capacity;
    int sizeof_element;
} cvector;
void *cvector_init(cvector *self, int sizeof_element);
void *cvector_free(cvector *self);
void *cvector_resize(cvector *self, int size);
void *cvector_push_back(cvector *self, void *element);
void *cvector_pop(cvector *self);
#endif