CC=gcc
CFLAGS=-std=c99 -O2 -lm

a.out: src/main.c src/ckt/pwm.c
	$(CC) -o a.out src/main.c src/ckt/pwm.c $(CFLAGS)

clean:
	rm a.out
