CC=gcc
CFLAGS=-std=c99 -O2 -lm -g -Wall

a.out: src/main.c src/ckt/pwm.c src/ckt/pfc.c src/ckt/logger.c
	$(CC) -o a.out src/main.c src/ckt/pwm.c src/ckt/pfc.c src/ckt/logger.c $(CFLAGS)

clean:
	rm -f a.out out.txt
