#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include "value.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"
#include "utility.h"

int main(int argc, char **argv) { 
	int d = 0;
	FILE *fp = stdin;
	value *v = newIntegerValue(0);
	queue *infix = createQueue();
	queue *temp = createQueue();
	queue *postfix = createQueue();
	tNode *head = NULL;
	if(argc > 1){									//handles -d, -v, or read from file option
		if(strcmp(argv[1], "-v")==0){				//handle -v option
			printf("Joseph G. Murphy\n");
			return 0;
		}
		else if(strcmp(argv[1],"-d")==0) d=1;		//sets flag for the -d option
		else {										//else arg must be file
			fp = fopen(argv[argc-1],"r"); 
			if(fp ==0) Fatal("File not open\n");
			v = readValue(fp,head);
			while(!feof(fp)){
				if (v == NULL) break;
				if(v->type == VAR){
					if(head == NULL) head = addVar(fp,head);
					else addVar(fp,head);
				}
				if(v->type == SEMICOLON)	temp = clearQueue(infix,head,d);
				else	enqueue(v,infix);
				v = readValue(fp,head);
			}
		}
	}
	if(argc > 2) {									//handles reading in from file and -d option
		if(strcmp(argv[1],"-d")==0) d=2;			//sets flag for the -d option
		fp = fopen(argv[argc-1],"r"); 
		if(fp ==0) Fatal("File not open");
		v = readValue(fp,head);
		while(!feof(fp)){
			if (v == NULL) break;
			if(v->type == VAR){
				if(head == NULL) head = addVar(fp,head);
				else addVar(fp,head);
			}
			if(v->type == SEMICOLON)	temp = clearQueue(infix,head,d);
			else	enqueue(v,infix);
			v = readValue(fp,head);
		}
	}
	if(argc == 1 || d == 1) {						//handles reading in for stdin
		v = readValue(fp,head);
		while(!feof(fp)) {
			if(v->type == VAR){
				if(head == NULL) head = addVar(fp,head);
				else addVar(fp,head);
			}
			if(v->type == SEMICOLON)	temp = clearQueue(infix,head,d);
			else	enqueue(v,infix);
			v = readValue(fp,head);
		}
	}
	postfix = infixToPostfix(temp); 
	if(d==1 || d == 2){								//handles the -d option
		while(postfix->head != NULL){
			printValue(stdout,dequeue(postfix));
		}
	}
	else if(postfix->head != NULL)	evaluatePostfix(postfix,head,1);
	printf("\n");
	fclose(fp);
	return 0;
}
