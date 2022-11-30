all : samples

samples : samples.o
	gcc -o samples samples.o -Wall -Wextra

samples.o : samples.c
	gcc -c samples.c -Wall -Wextra

clean :
	rm  samples *.o 