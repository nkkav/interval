CC = gcc
CFLAGS =-O2 -Wall
EXE = .exe

all: test-interval$(EXE)

test-interval$(EXE): interval.o test-interval.o
	$(CC) interval.o test-interval.o -o test-interval$(EXE)

interval.o: interval.c interval.h
	$(CC) $(CFLAGS) -c interval.c

test-interval.o: test-interval.c interval.h
	$(CC) $(CFLAGS) -c test-interval.c

tidy:
	rm -f *.o

clean:
	rm -f *.o test-interval$(EXE)
