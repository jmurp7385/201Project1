/*
 * This is the header file for the stack data structure
 * Made by Joey Murphy
 */
#ifndef STACK_H
#define STACK_H
#include "value.h"

typedef struct sNode {
	value *v;
	struct sNode *next;
}sNode;

typedef struct stack {
	int size;
	sNode *head;
}stack;

sNode *newNodeS(value *);
stack *createStack(void);
void push(value *,stack *);
value *pop(stack *);
value *peekS(stack *);
value *topStack(stack *);
value *bottomStack(stack *);

#endif
