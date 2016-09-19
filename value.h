/*
 * This is the header file for the value data struct
 */
#ifndef VALUE_H
#define VALUE_H

typedef struct Value{
    char *type;
    char *varType;
    char *name;
    int ival;
    double rval;
    char *sval;
}value;

extern char *INTEGER;
extern char *DOUBLE;
extern char *STRING;
extern char *VARIABLE;
extern char *VAR;
extern char *OPERATOR;
extern char *SEMICOLON;

extern value *newIntegerValue(int);
extern value *newRealValue(double);
extern value *newStringValue(char *);
extern value *newVariableValue(char *,value *);
extern value *newOperatorValue(char *);
extern value *newSemiColonValue(char *);
void Fatal(char *, ...);
#endif
