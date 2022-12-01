all : txt2epub

txt2epub : txt2epub.o
	gcc -o txt2epub txt2epub.o -Wall -Wextra

txt2epub.o : txt2epub.c
	gcc -c txt2epub.c -Wall -Wextra

clean :
	rm  txt2epub *.o 
	rm  *.epub
	rm  ebooks.zip