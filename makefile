all: rpi_monitor_temp

rpi_monitor_temp: temp.o main.o
	gcc -o rpi_monitor_temp temp.o main.o

temp.o: ./sources/temp.c
	gcc -o temp.o -c ./sources/temp.c -W -Wall -ansi -pedantic

main.o: ./sources/main.c ./headers/temp.h
	gcc -o main.o -c ./sources/main.c -W -Wall -ansi -pedantic

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello
