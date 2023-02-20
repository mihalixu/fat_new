all: main

main.o: main.c fat.c partition.c add.c put.c delete.c
	gcc main.c fat.c partition.c add.c put.c delete.c -c 

clean: 
     rm *.o