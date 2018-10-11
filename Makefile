all: main

main : main.o token.o error.o syntax.o interpret.o semantics.o complete.o
	gcc -o $@ $^ -lm

.c.o:
	gcc -c $< 

clean:
	rm *.o
