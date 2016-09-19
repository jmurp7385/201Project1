/*
 * This is the .c file for the value data structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"
#include "utility.h"

extern void Fatal(char *fmt,...);

static value *newValue(char *t);

char *INTEGER = "INTEGER";
char *DOUBLE = "DOUBLE";
char *STRING = "STRING";
char *VARIABLE = "VARIABLE";
char *VAR = "VAR";
char *OPERATOR = "OPERATOR";
char *SEMICOLON = "SEMICOLON";
char *ENDOFFILE = "ENDOFFILE";

value *newIntegerValue(int i){
    value *v = newValue(INTEGER);
    v->ival = i;
	return v;
}

value *newRealValue(double i){
    value *v = newValue(DOUBLE);
    v->rval = i;
	return v;
}

value *newStringValue(char *i){
    value *v = newValue(STRING);
    v->sval = i;
	return v;
}

value *newVariableValue(char *i, value *val){
    value *v = newValue(VARIABLE);
    v->name = i;
	v->varType = val->type;
	if(isInteger(val)) v->ival = val->ival;
	if(isReal(val)) v->rval = val->rval;
	if(isString(val)) v->sval = val->sval;
	return v;
}

value *newOperatorValue(char *i){
    value *v = newValue(OPERATOR);
    v->sval = i;
	return v;
}

value *newSemiColonValue(char *i){
    value *v = newValue(SEMICOLON);
    v->sval = i;
	return v;
}

static value *newValue(char *t){
    value *v = (value*)malloc(sizeof(value));
    if((v = malloc(sizeof(value))) == 0){
        printf("out of memory\n");
	}
    v->type = t;
	v->varType = NULL;
    v->name  = NULL;
	v->ival = 0;
    v->rval = 0;
    v->sval = NULL;
    return v;
}

void Fatal(char *fmt, ...){
	va_list ap;
	fprintf(stderr,"An error occured: ");
	va_start(ap,fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(-1);
}
