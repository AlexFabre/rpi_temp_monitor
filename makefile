CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix/
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: rpi_monitor_temp

rpi_monitor_temp: temp.o main.o $(RGB_LIBRARY)
	$(CC) -o rpi_monitor_temp temp.o main.o $(LDFLAGS) -lstdc++

temp.o: ./sources/temp.c
	$(CC) -o temp.o -c ./sources/temp.c -W -Wall -ansi -pedantic

main.o: ./sources/main.c ./headers/temp.h
	$(CC) -o main.o -I$(RGB_INCDIR) $(CFLAGS) -c ./sources/main.c -W -Wall -ansi -pedantic

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello
