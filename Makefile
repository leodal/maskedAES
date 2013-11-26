CC=gcc
CFLAGS=-Wall -ansi -DDEBUG -DKEY_SIZE=192 -DLINEAR_SIZE=2

SHARES=5
ROUNDS=2

all: genTables tests

tests: test_shares test_aesLike

test_aesLike: aesLike.o gf256.o test_aesLike.c
	$(CC) $(CFLAGS) $^ -o $@

aesLike.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DNB_ROUNDS=$(ROUNDS) -c $<

shares.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) -c $<

secureAESlike.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) -DNB_ROUNDS=2 -c $<

genTables: aes.o gf256.o genTables.c
	$(CC) $(CFLAGS) $^ -o $@

test_aes: aes.o test_aes.c
	$(CC) $(CFLAGS) $^ -o $@

test_secureAES: gf256.o shares.o secureAESlike.o test_secureAES.c
	$(CC) $(CFLAGS) -DNB_ROUNDS=$(ROUNDS) $^ -o $@

test_shares: shares.o gf256.o test_shares.c
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) $^ -o $@

test_secureOps: gf256.o shares.o secureOps.o test_secureOps.c
	$(CC) $(CFLAGS) $^ -o $@

test: gf256.o shares.o secureOps.o aes.o test.o
	$(CC) $(CFLAGS) $^ -o $@

run_test: test
	./test

clean:
	-rm -f *~ *.o test test_shares genTables test_aesLike test_secureAES test_aes