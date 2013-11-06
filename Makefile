lCC=gcc
CFLAGS=-Wall

all: test

test: gf256.o shares.o secureOps.o test.o
	$(CC) $^ -o $@

run_test: test
	./test

clean:
	-rm -f *~ *.o test