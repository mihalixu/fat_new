all: main

main: main.o fat.o partition.o add.o put.o delete.o
	gcc main.o fat.o partition.o add.o put.o delete.o -o main

clean:
	rm *.o main 