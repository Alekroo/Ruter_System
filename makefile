CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g
BINS=test

test: main.c
	$(CC) $(CFLAGS) $^ -o $@

	
