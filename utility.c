/*
 * This is the source file for utility functions
 * Contains accessory functions for calculon
 * Made by Joey Murphy
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include "bst.h"
#include "value.h"
#include "queue.h"
#include "stack.h"
#include "scanner.h"
#include "utility.h"

static void addition(value *,value *,stack *,tNode *);
static void subtraction(value *,value *,stack *,tNode *);
static void multiplication(value *,value *,stack *,tNode *);
static void division(value *,value *,stack *,tNode *);
static void modulus(value *,value *,stack *,tNode *);
static void exponentiation(value *,value *,stack *,tNode *);
static void assign(value *,value *,stack *,queue *,tNode *);
static char *concat(value *,value *);
static int priority(value *);
static value *getVarValue(value *,tNode *);
static int isParenthesis(char *);
static int isParenthesisV(value *);
static int isVar(char *);
static int isPlus(char *);
static int isMinus(char *);
static int isMultiply(char *);
static int isDivide(char *);
static int isMod(char *);
static int isExponent(char *);

queue *infixToPostfix(queue *q) {
	queue *postfix = createQueue();
	stack *s = createStack();
	while(q->head != NULL) {
		if(!isOperator(q->head->v) && !isParenthesisV(q->head->v))
			enqueue(dequeue(q),postfix);
		else if (isParenthesisV(q->head->v)) {
			if (strcmp(q->head->v->sval,"(") == 0)
				push(dequeue(q),s);
			else {
				while (s->head != NULL && strcmp(s->head->v->sval,"(") != 0)
					enqueue(pop(s),postfix);
				pop(s);
				dequeue(q);
			}
		}
		else if (isOperator(q->head->v) && s->head == NULL)
			push(dequeue(q),s);
		else {
			if (!isParenthesisV(s->head->v)) {
				while (s->head != NULL && priority(q->head->v) <= priority(s->head->v))
					enqueue(pop(s),postfix);
				push(dequeue(q),s);
			}
			else
				push(dequeue(q),s);
		}
	}
	if (s->head != NULL) {		//enqueues any remaining operators on the stack
		while(s->head != NULL){
			if(!isParenthesisV(s->head->v))
				enqueue(pop(s),postfix);
			else pop(s);
		}
	}
	return postfix;
}

value *evaluatePostfix(queue *q, tNode *h,int p){
	stack *s = createStack();
	value *v, *v1, *v2;
	v = v1 = v2 = NULL;
	if(q->size > 1) {
	while(q->head != NULL){
		v = dequeue(q);
		if(!isOperator(v) && v->type != SEMICOLON){
			push(v,s);
		}
		else {
			v1 = pop(s);
			v2 = pop(s);
			if(isOperator(v)) {
				if(isPlus(v->sval))		addition(v2,v1,s,h);
				if(isMinus(v->sval))	subtraction(v2,v1,s,h);
				if(isMultiply(v->sval))	multiplication(v1,v2,s,h);
				if(isDivide(v->sval)) 	division(v2,v1,s,h);
				if(isMod(v->sval))		modulus(v1,v2,s,h);
				if(isExponent(v->sval))	exponentiation(v1,v2,s,h);
				if(isEqual(v->sval))	assign(v1,v2,s,q,h);
			}
		}
	}
	v = pop(s);
	v = getVarValue(v,h);
	if(p == 1)
		printValue(stdout,v);
	}
	else {
		if(p == 1){
			v = getVarValue(dequeue(q),h);
			printValue(stdout,v);
		}
	}
	if(q->size == 1) v = dequeue(q);
	return v;
}

//Modified from Professor Lusth's sample
value *readValue(FILE *fp,tNode *h) {
	value *v;
	tNode *n;
	if (stringPending(fp)) {		//reads in an double quoted string
		char *t = readString(fp);
		if(t == 0) return NULL;
		v = newStringValue(t);
	}
	else {
		char *t = readToken(fp);
		if(t == 0) return NULL;
		if(strchr(t,'.') != 0) 		//dot found - must be a real number
			v = newRealValue(atof(t));
		else if ((t[0] == '-' && (isdigit(t[1]) && strlen(t) > 1)) || isdigit(*t)) 
			v = newIntegerValue(atoi(t));
		else if (isPlus(t) || isMinus(t) || isMultiply(t) ||
			isDivide(t) || isMod(t) || isExponent(t) || isEqual(t)) 
			v = newOperatorValue(t);
		else if(isParenthesis(t))
			v = newOperatorValue(t);
		else if(isSemiColon(t))
			v = newSemiColonValue(t);
		else if(isVar(t)) {			//add var to tree here
			v = newIntegerValue(0);
			v->type = VAR;
		}
		else if(isalpha(*t)){
			n = search(h,t);
			if(n != NULL) v = n->v;
			else Fatal("%s is not a variable\n",t);
			return v;
		}
		else {
			Fatal("The token %s is not a value\n",t);
			v = NULL;
		}
	}
	return v;
}

tNode *addVar(FILE *fp, tNode *h){
	char *var  = readToken(fp);     //gets variable name
	readToken(fp);					//throws away '='
	value *val  = readValue(fp,h);  
	value *v = newVariableValue(var,val);
	tNode *n =	newNodeT(v);
	return insert(n,h);
}

queue *clearQueue(queue *q,tNode *h,int d) {
	queue *temp = createQueue();
	queue *temp1 = createQueue();
	queue *e = createQueue();
	value *var = NULL;
	value *update = NULL;
	value *v = NULL;
	if(d == 0){
	while(q->head != NULL) {
		v = dequeue(q);
		enqueue(v,temp1);
		enqueue(v,e);
	}
	if (isVariable(peek(e)) && e->size > 1){
		var = dequeue(e);
		value *op = dequeue(e);
		if(isEqual(op->sval)){
			e = infixToPostfix(e);
			update = evaluatePostfix(e,h,0);
			update = newVariableValue(var->name,update);
			tNode *t = newNodeT(update);
			insert(t,h);
		}
		else { 
			enqueueFront(op,e); 
			enqueueFront(var,e);
		}
	}
	return temp1;
	}
	while(q->head != NULL) {
		v = dequeue(q);
		enqueue(v,temp);
	}
	return temp;
}

//Modified from Professor Lusth's sample
void printValue(FILE *fp,value *v) {
	if (isInteger(v))
		fprintf(fp,"%d ",v->ival);
	else if(isReal(v))
		fprintf(fp,"%f ",v->rval);
	else if(isVariable(v))
		fprintf(fp,"%s ",v->name);
	else if(isOperator(v))
		fprintf(fp,"%s ",v->sval);
	else //must be a string
		fprintf(fp,"\"%s\" ",v->sval);
}

static int priority(value *v) {
	char *o = v->sval;
	if(isEqual(o))				return 0;
	else if(isPlus(o))			return 1;
	else if(isMinus(o))			return 2;
	else if(isMultiply(o))		return 3;
	else if(isDivide(o))		return 4;
	else if(isMod(o))			return 5;
	else if(isExponent(o))		return 6;
	else
		return -1;
}

static value *getVarValue(value *v,tNode *h){
	if(isVariable(v)){
		value *temp = NULL ;
		tNode *n = search(h,v->name);
		temp = n->v;
		if(temp->varType == INTEGER)
			temp = newIntegerValue(temp->ival);
		if(temp->varType == DOUBLE)
			temp = newRealValue(temp->rval);
		if(temp->varType == STRING)
			temp = newStringValue(temp->sval);
		return temp;	
	}
	return v;
}

static void updateValue(value *v,value *new,tNode *h){
			value *u = newVariableValue(v->name,new);
			tNode *t = newNodeT(u);
			insert(t,h);
}

static void assign(value *x,value *y,stack *s,queue *q,tNode *h){
		updateValue(y,x,h);
		push(x,s);
}


static void addition(value *x,value *y,stack *s,tNode *h){
	value *v1 = getVarValue(x,h);
	value *v2 = getVarValue(y,h);
	if(isInteger(v1) && isInteger(v2)) 
		push(newIntegerValue(v1->ival+v2->ival),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(v1->rval+v2->ival),s);
	if(isInteger(v1) && isReal(v2)) 
		push(newRealValue(v1->ival+v2->rval),s);
	if(isReal(v1) && isReal(v2)) 
		push(newRealValue(v1->rval+v2->rval),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(v1->ival+atoi(v2->sval)),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(atoi(v1->sval)+v2->ival),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(v1->rval+atof(v2->sval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(atof(v1->sval)+v2->rval),s);
	if(isString(v1) && isString(v2))
		push(newStringValue(concat(v1,v2)),s);
}

static void subtraction(value *v1,value *v2,stack *s,tNode *h){
	v1 = getVarValue(v1,h);
	v2 = getVarValue(v2,h);
	if(isInteger(v1) && isInteger(v2)) 
		push(newIntegerValue(v1->ival-v2->ival),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(v1->rval-v2->ival),s);
	if(isInteger(v1) && isReal(v2)) 
		push(newRealValue(v1->ival-v2->rval),s);
	if(isReal(v1) && isReal(v2)) 
		push(newRealValue(v1->rval-v2->rval),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(v1->ival-atoi(v2->sval)),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(atoi(v1->sval)-v2->ival),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(v1->rval-atof(v2->sval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(atof(v1->sval)-v2->rval),s);
}

static void multiplication(value *v1,value *v2,stack *s,tNode *h) {
	v1 = getVarValue(v1,h);
	v2 = getVarValue(v2,h);
	if(isInteger(v1) && isInteger(v2)) 
		push(newIntegerValue(v1->ival*v2->ival),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(v1->rval*v2->ival),s);
	if(isInteger(v1) && isReal(v2)) 
		push(newRealValue(v1->ival*v2->rval),s);
	if(isReal(v1) && isReal(v2)) 
		push(newRealValue(v1->rval*v2->rval),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(v1->ival*atoi(v2->sval)),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(atoi(v1->sval)*v2->ival),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(v1->rval*atof(v2->sval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(atof(v1->sval)*v2->rval),s);

}

static void division(value *v1,value *v2,stack *s,tNode *h) {
	v1 = getVarValue(v1,h);
	v2 = getVarValue(v2,h);
	if(isInteger(v1) && isInteger(v2))
		push(newIntegerValue((v1->ival/v2->ival)),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(v1->rval/v2->ival),s);
	if(isInteger(v1) && isReal(v2)) 
		push(newRealValue(v1->ival/v2->rval),s);
	if(isReal(v1) && isReal(v2)) 
		push(newRealValue(v1->rval/v2->rval),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(v1->ival/atoi(v2->sval)),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(atoi(v1->sval)/v2->ival),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(v1->rval/atof(v2->sval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(atof(v1->sval)/v2->rval),s);
}

static void modulus(value *v1,value *v2,stack *s,tNode *h) {
	v1 = getVarValue(v1,h);
	v2 = getVarValue(v2,h);
	if(isInteger(v1) && isInteger(v2))
		push(newIntegerValue(v2->ival%v1->ival),s);
	if(isInteger(v1) && isReal(v2))
		push(newRealValue(fmod(v2->rval,v1->ival)),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(fmod(v2->ival,v1->rval)),s);
	if(isReal(v1) && isReal(v2))
		push(newRealValue(fmod(v2->rval,v1->rval)),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(atoi(v2->sval)%v1->ival),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(v2->ival%atoi(v1->sval)),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(fmod(atof(v2->sval),v1->rval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(fmod(v2->rval,atof(v1->sval))),s);
}

static void exponentiation(value *v1,value *v2,stack *s,tNode *h){
	v1 = getVarValue(v1,h);
	v2 = getVarValue(v2,h);
	if(isInteger(v1) && isInteger(v2))
		push(newIntegerValue(pow(v2->ival,v1->ival)),s);
	if(isInteger(v1) && isReal(v2))
		push(newRealValue(pow(v2->rval,v1->ival)),s);
	if(isReal(v1) && isInteger(v2))
		push(newRealValue(pow(v2->ival,v1->rval)),s);
	if(isReal(v1) && isReal(v2))
		push(newRealValue(pow(v2->rval,v1->rval)),s);
	if(isInteger(v1) && isString(v2))
		push(newIntegerValue(pow(atoi(v2->sval),v1->ival)),s);
	if(isString(v1) && isInteger(v2))
		push(newIntegerValue(pow(v2->ival,atoi(v1->sval))),s);
	if(isReal(v1) && isString(v2))
		push(newRealValue(pow(atof(v2->sval),v1->rval)),s);
	if(isString(v1) && isReal(v2))
		push(newRealValue(pow(v2->rval,atof(v1->sval))),s);
}

static char *concat(value *v1, value *v2){
	char *c = malloc(sizeof(char) * (strlen(v1->sval) + strlen(v2->sval) + 1));
	if(c == 0) Fatal("Out of Memory\n");
	sprintf(c,"%s%s",v1->sval,v2->sval);
	return c;
}

int isOperator(value *v) { 
	return v->type == OPERATOR;
}	
int isInteger(value *v){
	return v->type == INTEGER;
}
int isReal(value *v){
	return v->type == DOUBLE;
}
int isString(value *v){
	return v->type == STRING;
}
int isVariable(value *v) {
	return v->type == VARIABLE;
}
static int isParenthesis(char *o) {
	return (strcmp(o,"(") == 0) || (strcmp(o,")") == 0);
}
static int isParenthesisV(value *v) {
	if (v->sval == 0 ) return 1 == 2;
	else
		return (strcmp(v->sval,"(") == 0) || (strcmp(v->sval,")") == 0);
}
int isSemiColon(char *o) {
	return strcmp(o,";") == 0;
}
static int isVar(char *o) {
	return strcmp(o,"var") == 0;
}
int isEqual(char *o){
	return strcmp(o,"=") == 0;
}
static int isPlus(char *o){
	return strcmp(o,"+") == 0;
}
static int isMinus(char *o){
	return strcmp(o,"-") == 0;
}
static int isMultiply(char *o){
	return strcmp(o,"*") == 0;
}
static int isDivide(char *o){
	return strcmp(o,"/") == 0;
}
static int isMod(char *o){
	return strcmp(o,"%") == 0;
}
static int isExponent(char *o){
	return strcmp(o,"^") == 0;
}
