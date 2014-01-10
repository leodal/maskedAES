CC=gcc
CFLAGS=-Wall -ansi

SHARES=5

all: tests bench_aesLike genTables

aesLike_2_2.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS)  -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -c $< -o $@

aesLike_16_10.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -c $< -o $@

secureAESlike_2_2_5.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=5 -c $< -o $@

secureAESlike_16_10_5.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DSHARES=5 -c $< -o $@

shares_5.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) -c $< -o $@

sbox_tools.o: sbox_tools.c sbox_tools.h
	$(CC) $(CFLAGS) -c $< -o $@

bench_aesLike: aesLike_16_10.o sbox_tools.o secureAESlike_16_10_5.o shares_5.o gf256.o bench_aesLike.c
	$(CC) $(CFLAGS) $^ -o $@

test_aesLike: aesLike_2_2.o sbox_tools.o gf256.o test_aesLike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 $^ -o $@

test_sbox_tools: sbox_tools.o gf256.o test_sbox_tools.c
	$(CC) $(CFLAGS) $^ -o $@

test_shares: shares.o gf256.o test_shares.c
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) $^ -o $@

test_secureAESlike: shares_5.o gf256.o secureAESlike_2_2_5.o test_secureAESlike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=5 $^ -o $@

tests: test_aesLike test_sbox_tools test_shares test_secureAESlike

genTables: gf256.o genTables.c
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

test_secureAESlike_debug: shares_5.o gf256.o secureAESlike_2_2_5.o test_secureAESlike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=5 -DDEBUG $^ -o $@

clean:	
	-rm -f *~ *.o {bench,test}_aesLike{_debug,} test_sbox_tools{_debug,} genTables test_secureAESlike{_debug,} test_shares

# Les vieux trucs

test_secureOps: gf256.o shares_5.o secureOps.o test_secureOps.c 
	$(CC) $(CFLAGS) $^ -o $@

