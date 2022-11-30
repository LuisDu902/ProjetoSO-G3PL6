all : tokenring

tokenring : tokenring.o
	gcc -o tokenring tokenring.o -Wall -Wextra

tokenring.o : tokenring.c
	gcc -c tokenring.c -Wall -Wextra

clean :
	rm  tokenring *.o 
	rm  *pipe*  