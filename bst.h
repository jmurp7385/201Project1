/*
 * This is the header file for the Binary Source Tree data Struct
 * Made By Joey Murphy
 */
#ifndef BST_H
#define BST_H
#include "value.h"

typedef struct tNode {
	char *key;
	value *v;
	struct tNode *left;
	struct tNode *right;
 }tNode;

tNode *newNodeT(value *);
tNode *insert(tNode *,tNode *);
tNode *search(tNode *, char *);
void inOrder(tNode *);
#endif 
