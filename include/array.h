#ifndef __CUSTOM_ARRAY
#define __CUSTOM_ARRAY

typedef struct {
    int* array;
    size_t used;
    size_t size;
} Array;

Array* initArray(size_t initialSize) {
    Array *a = (Array*) calloc(sizeof(Array), 1);

    a->array = (int *) calloc(sizeof(int), initialSize);
    a->used = 0;
    a->size = initialSize;

    return a;
}

void insertArray(Array *a, int element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (int *) realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void printArray(Array* a) {
    int i, j = a->used;

    for(i = 0; i < j; i++) {
        printf("%d, %d\n", a->array[i] / 10, a->array[i] % 10);
    }
}

#endif