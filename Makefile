all: main

main : main.o token.o error.o
	gcc -o $@ $^ 

.c.o:
	gcc -c $< 

clean:
	rm *.o
