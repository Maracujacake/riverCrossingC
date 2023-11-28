CC=gcc

default: program

main=./main.c

font=./*.c

options=-lpthread

program:
	$(CC) $(font) $(main) $(options) -o main.o
	./main.o # executando

clean:
	rm -f main.o