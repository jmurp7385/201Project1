/*
 * This is the .c file for the Queue Data structure
 * MADE BY JOEY MURPHY
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

queue *createQueue(){
	queue* temp = (queue*)malloc(sizeof(queue));
	temp->head = temp->tail = (qNode*)malloc(sizeof(qNode));
	temp->size = 0;
	return temp;
}

int isEmpty(queue *q){
    if(q->size == 0)
        return 1;
    return 0;
}

void enqueue(value *v, queue *q){
    qNode *temp = (qNode*)malloc(sizeof(qNode));
    temp->v = v;
    temp->next = NULL;
    if(isEmpty(q) == 1){
        q->head = q->tail = temp;
		q->size = 1;
		return;
    }
    else{
        q->tail->next = temp;
        q->tail = q->tail->next;
		q->tail->next = NULL;
    }
	q->size++;
}

void enqueueFront(value *v, queue *q) {
    qNode *new = (qNode*)malloc(sizeof(qNode));
    new->v = v;
    new->next = NULL;
	qNode *temp = NULL;
	if (isEmpty(q) == 1){
        q->head = q->tail = temp;
		q->size = 1;
		return;
    }
	else {
		temp = q->head;
		q->head = new;
		q->head->next = temp;
	}
	q->size++;
}

value *dequeue(queue *q){
    qNode *temp = NULL;
	temp = q->head;
	value *v = NULL;
   	if(q->size == 1){
		q->head = NULL;
	}
	else{
		q->head = q->head->next;
	}
	q->size--;
	v = temp->v;
    return v;
}   
 
value *peek(queue *q){
    if(isEmpty(q) == 0){
        value *j = q->head->v;
        return j;
	}
    else{
        return NULL;
    }
}   
