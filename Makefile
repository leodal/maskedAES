CC=gcc
CFLAGS=-Wall -ansi -DKEY_SIZE=192
SHARES=5
ROUNDS=2

all: test_aesLike
	./test_aesLike && echo "Test OK"

aesLike_2_2.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS)  -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -c $< -o $@

aesLike_16_10.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -c $< -o $@

sbox_tools.o: sbox_tools.c sbox_tools.h
	$(CC) $(CFLAGS) -c $< -o $@

bench_aesLike: aesLike_16_10.o gf256.o bench_aesLike.c
	$(CC) $(CFLAGS) $^ -o $@

test_aesLike: aesLike_2_2.o sbox_tools.o gf256.o test_aesLike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 $^ -o $@

test_sbox_tools: sbox_tools.o gf256.o test_sbox_tools.c
	$(CC) $(CFLAGS) $^ -o $@

# Version debug (messages de traçage activés, en fait)
aesLike_2_2_debug.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DDEBUG -c $< -o $@

aesLike_16_10_debug.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DDEBUG -c $< -o $@

sbox_tools_debug.o: sbox_tools.c sbox_tools.h
	$(CC) $(CFLAGS) -DDEBUG_LAGRANGE -c $< -o $@

bench_aesLike_debug: aesLike_16_10.o gf256.o bench_aesLike.c
	$(CC) $(CFLAGS) -DDEBUG $^ -o $@

test_aesLike_debug: aesLike_2_2_debug.o gf256.o test_aesLike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DDEBUG $^ -o $@

clean:	
	-rm -f *~ *.o {bench,test}_aesLike{_debug,} test_sbox_tools{_debug,}

# Les vieux trucs
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