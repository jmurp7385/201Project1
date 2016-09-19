/*
 * This is the header file for the Queue data structure 
 * MADE BY JOEY MURPHY
 */
#ifndef QUEUE_H
#define QUEUE_H
#include "value.h"

typedef struct qNode {
	value *v;
	struct qNode *next;
}qNode;

typedef struct queue {
	int size;
	qNode *head;
	qNode *tail;
}queue;

extern qNode* head;

queue *createQueue(void);
int isEmpty(queue*);
void enqueue(value *, queue*);
void enqueueFront(value *, queue*);
value *dequeue(queue*);
value *peek(queue*);
#endif
