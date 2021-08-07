main: clean main.o
	gcc -L/usr/local/lib -o main main.o -lsdl2
	./main
main.o: main.c
	gcc -Wall -c main.c
clean:
	rm -rf **.o