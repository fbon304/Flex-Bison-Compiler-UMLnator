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

void destroyStack(Stack* s) {
    free(s);
}

void initStack(Stack *s) {
    s->top = -1;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

bool isFull(Stack *s) {
    return s->top == MAX - 1;
}

bool push(Stack *s, char * value) {
    if (isFull(s)) {
        printf("Stack overflow! Can't insert %d\n", value);
        return false;
    }
    s->items[++(s->top)] = value;
    return true;
}

char * pop(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack underflow! No se puede hacer pop\n");
        return -1;
    }
    return s->items[(s->top)--];
}

char * peek(Stack *s) {
    if (isEmpty(s)) {
        printf("La pila está vacía\n");
        return -1;
    }
    return s->items[s->top];
}