main: clean main.o
	gcc -L/usr/local/lib -o main main.o -lsdl2 -lsdl2_ttf
	./main
main.o: main.c
	gcc -Wall -c main.c
clean:
	rm -rf **.o