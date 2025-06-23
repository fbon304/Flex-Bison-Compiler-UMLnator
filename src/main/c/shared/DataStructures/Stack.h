#ifndef STACK_H
#define STACK_H

#include "../Type.h"

#define MAX 100

typedef struct {
    char * items[MAX];
    int top;
} Stack;

Stack* createStack(void);

void destroyStack(Stack* s);

void initStack(Stack *s);

boolean isEmpty(Stack *s);

boolean isFull(Stack *s);

boolean push(Stack *s, char * value);

char * pop(Stack *s);

char * peek(Stack *s);

#endif // STACK_H
