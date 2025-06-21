#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* createStack(void) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (s != NULL) {
        s->top = -1;
    }
    return s;
}

void destroyStack(Stack * s) {
    free(s);
}

void initStack(Stack * s) {
    s->top = -1;
}

boolean isEmpty(Stack * s) {
    return s->top == -1;
}

boolean isFull(Stack * s) {
    return s->top == MAX - 1;
}

boolean push(Stack * s, char * value) {
    if (isFull(s)) {
        //printf("Stack overflow! Can't insert %s\n", value);
        return false;
    }
    s->items[++(s->top)] = value;
    return true;
}

char * pop(Stack * s) {
    if (isEmpty(s)) {
        //printf("Stack underflow! No se puede hacer pop\n");
        return NULL;
    }
    return s->items[(s->top)--];
}

char * peek(Stack * s) {
    if (isEmpty(s)) {
        //printf("La pila está vacía\n");
        return NULL;
    }
    return s->items[s->top];
}