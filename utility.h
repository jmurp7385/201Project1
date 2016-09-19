/*
 * This is the source file for utility functions
 * Made by Joey Murphy
 *
 */
#ifndef UTILITY_H
#define UTILITY_H

queue *infixToPostfix(queue *);
value *readValue(FILE *,tNode *);
value *evaluatePostfix(queue *,tNode *h,int);
void printValue(FILE *,value *);
tNode *addVar(FILE *,tNode *);
queue *clearQueue(queue *,tNode*,int);
int isOperator(value *);
int isInteger(value *);
int isReal(value *);
int isString(value *);
int isVariable(value *);
int isEqual(char *);
int isSemiColon(char *);

#endif
