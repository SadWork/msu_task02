#include "cvector.h"
#define INITIAL_CAPACITY 10

typedef struct
{
    void *data;
    int size;
    int capacity;
    int sizeof_element;
} cvector;

void *cvector_init(cvector *self, int sizeof_element)
{
    self->data = malloc(INITIAL_CAPACITY * sizeof_element);
    if (self->data == NULL)
    {
        ERROR_MSG("malloc return NULL");
        exit(1);
    }

    self->size           = 0;
    self->sizeof_element = sizeof_element;
    self->capacity       = INITIAL_CAPACITY;
    return self->data;
}

void *cvector_free(cvector *self)
{
    free(self->data);
    self->size     = 0;
    self->capacity = 0;
    return NULL;
}

void *cvector_resize(cvector *self, int size)
{
    if (self->capacity < size)
    {
        self->capacity = 2 * size;
        void *tmp      = realloc(self->data, self->capacity * self->sizeof_element);
        if (tmp == NULL)
        {
            cvector_free(self);
            ERROR_MSG("realloc return NULL");
            exit(1);
        }
        self->data = tmp;
    }
    self->size = size;
    return self->data;
}
void *cvector_push_back(cvector *self, void *element)
{
    if (self->size == self->capacity)
    {
        self->capacity *= 2;
        void *tmp = realloc(self->data, self->capacity * self->sizeof_element);
        if (tmp == NULL)
        {
            cvector_free(self);
            ERROR_MSG("realloc return NULL");
            exit(1);
        }
        self->data = tmp;
    }

    memcpy((char *)(self->data) + self->sizeof_element * self->size, element, self->sizeof_element);
    self->size++;
    return self->data;
}

void *cvector_pop(cvector *self)
{
    if (self->size <= self->capacity / 2 && self->capacity > INITIAL_CAPACITY)
    {
        self->capacity /= 2;
        void *tmp = realloc(self->data, self->capacity * self->sizeof_element);
        if (tmp == NULL)
        {
            cvector_free(self);
            ERROR_MSG("realloc return NULL");
            exit(1);
        }
        self->data = tmp;
    }
    self->size--;
    return self->data;
}