CC=gcc
CFLAGS=-Wall -ansi -DDEBUG -DKEY_SIZE=192 -DSHARES=1 -DLINEAR_SIZE=2

all: genTables tests

tests: test_shares run_aesLike

run_aesLike: aesLike.o gf256.o run_aesLike.c
	$(CC) $(CFLAGS) $^ -o $@

aesLike.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DNB_ROUNDS=2 -c $<

genTables: aes.o gf256.o genTables.c
	$(CC) $(CFLAGS) $^ -o $@

test_aes: aes.o debug_tools.o test_aes.c
	$(CC) $(CFLAGS) $^ -o $@

test_secureAES: gf256.o shares.o secureAES.o test_secureAES.c
	$(CC) $(CFLAGS) $^ -o $@

test_shares: shares.o gf256.o test_shares.c
	$(CC) $(CFLAGS) $^ -o $@

test_secureOps: gf256.o shares.o secureOps.o test_secureOps.c
	$(CC) $(CFLAGS) $^ -o $@

test: gf256.o shares.o secureOps.o aes.o test.o
	$(CC) $(CFLAGS) $^ -o $@

run_test: test
	./test

clean:
	-rm -f *~ *.o test test_shares genTables