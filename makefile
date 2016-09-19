OBJS= value.o stack.o queue.o scanner.o bst.o calculon.o utility.o
calculon:	$(OBJS)
	gcc -Wall -std=c99 -g $(OBJS) -o calculon -lm
calculon.o:	stack.h queue.h value.h bst.h calculon.c utility.h
	gcc -Wall -std=c99 -g -c calculon.c
value.o:	value.c value.h stack.h queue.h bst.h utility.h
	gcc -Wall -std=c99  -g -c value.c
stack.o:	stack.c stack.h value.h
	gcc -Wall -std=c99 -g -c stack.c
queue.o:	queue.c queue.h
	gcc -Wall -std=c99 -g -c queue.c
bst.o:	bst.c bst.h
	gcc -Wall -std=c99 -g -c bst.c
utility.o:	utility.c utility.h bst.h value.h queue.h stack.h scanner.h
	gcc -Wall -std=c99 -g -c utility.c
scanner.o:	scanner.c scanner.h
	gcc -Wall -std=c99 -g -c scanner.c
run:	calculon
	./calculon
test: calculon
	./calculon -v
	./calculon -d test
	./calculon -d test1
	./calculon -d test2
	./calculon -d test3
	./calculon -d test4
	./calculon -d test5
	./calculon test
	./calculon test1
	./calculon test2
	./calculon test3
	./calculon test4
	./calculon test5
clean:
	rm -f $(OBJS) calculon
prelim:
	wget beastie.cs.ua.edu/cs201/prelim1
	chmod +x prelim1
	prelim1 > REPORT
	rm prelim1

