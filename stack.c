/*
 *This is the source file for the Stack data structure
 *Made by Joey Murphy
 */
#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "stack.h"

stack *createStack(void){
	stack *temp = (stack*)malloc(sizeof(stack));
	temp->head = NULL;
	temp->size = 0;
	return temp;
}

sNode *newNodeS(value *v){
	sNode *temp = (sNode*)malloc(sizeof(sNode));
	temp->v = v;
	temp->next = NULL;
	return temp;
}

void push(value *v,stack *s){
	sNode *new = newNodeS(v); 
	sNode *temp = NULL;
	new->v = v;
	if(s->size == 0){
		s->head = new;
	}
	else{
		temp = s->head;
		s->head = new;
		s->head->next = temp;
	}
	s->size++;
}

value *pop(stack *s){
	sNode *temp = s->head;
	value *v = NULL;
	v = temp->v;
	if(s->size == 1)
		s->head = NULL;
	else
		s->head = s->head->next;
	s->size--;
	//free(temp);
	return v;
}

value *peekS(stack *s){
	value *temp = NULL;
	temp = pop(s);
	push(temp,s);
	return temp;
}
