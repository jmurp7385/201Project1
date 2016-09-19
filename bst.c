/*
 * This is the source file for the BST data structure
 * MADE BY JOEY MURPHY
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

tNode *newNodeT(value *v){
	tNode *temp = (tNode*)malloc(sizeof(tNode));
	temp->v = v;
	temp->key = v->name;
	temp->left = temp->right = NULL;
	return temp;
}

void inOrder(tNode * h){
	if(h != NULL){
		inOrder(h->left);
		printf("%s ",h->key);
		inOrder(h->right);
	}
}

tNode *insert(tNode *newNode, tNode *root){
	if(root == NULL) return root = newNode;
	if(strcmp(newNode->key,root->key)==0)  root->v = newNode->v;
	if(strcmp(newNode->key,root->key) < 0)
		root->left = insert(newNode, root->left);
	else if(strcmp(newNode->key,root->key) > 0)
		root->right = insert(newNode,root->right);
	else
		root = newNode;
	return root;
}

tNode *search(tNode *root, char *key){
	if(root == NULL) Fatal("%s is not a variable\n",key);
	else if (strcmp(root->key,key) > 0)
		return search(root->left,key);
	else if (strcmp(root->key,key) < 0)
		return search(root->right,key);
	else  
		return root;
	return NULL;
}
