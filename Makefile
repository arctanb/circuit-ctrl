CC=gcc
CFLAGS=-std=c99 -O4 -lm -g -Wall

SRC=src/main.c src/ckt/*.c src/model/*.c src/util/*.c src/ctrl/*.c

a.out: $(SRC)
	$(CC) -o a.out $(SRC) $(CFLAGS)

clean:
	rm -f a.out out.txt
