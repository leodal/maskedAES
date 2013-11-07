lCC=gcc
CFLAGS=-Wall -ansi

all: test test_aes genTables

test: gf256.o shares.o secureOps.o aes.o test.o
	$(CC) $^ -o $@

test_aes: aes.o debug_tools.o test_aes.c
	$(CC) $^ -o $@

genTables: gf256.o genTables.c
	$(CC) $^ -o $@

run_test: test
	./test

clean:
	-rm -f *~ *.o test