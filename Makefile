CC=gcc
CFLAGS=-Wall -Wextra -ansi

SHARES=3

all: tests bench_aesLikes genTables

# Compilation de l'AES-like avec les paramètres nécessaires pour nos programmes
# matrice 2x2, 2 tours
aesLike_2_2.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS)  -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -c $< -o $@

# matrice 16x16, 10 tours
aesLike_16_10.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -c $< -o $@

# Compilation de l'AES-like "sécurisé" avec les paramètres nécessaires pour nos programmes
# matrice 2x2, 0 tour, 3 shares
secureAESlike_2_0_3.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=0 -DSHARES=3 -c $< -o $@

# matrice 2x2, 1 tour, 1 share
secureAESlike_2_1_1.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=1 -DSHARES=1 -c $< -o $@

# matrice 2x2, 1 tour, 3 shares
secureAESlike_2_1_3.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=1 -DSHARES=3 -c $< -o $@

# matrice 2x2, 2 tours, 3 shares
secureAESlike_2_2_3.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=3 -c $< -o $@

# matrice 16x16, 10 tours, $(SHARES) shares 
secureAESlike_16_10.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DSHARES=$(SHARES) -c $< -o $@

# matrice 16x16, 10 tours, 1 shares
secureAESlike_16_10_1.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DSHARES=1 -c $< -o $@

# matrice 16x16, 10 tours, 3 shares
secureAESlike_16_10_3.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DSHARES=3 -c $< -o $@

# matrice 16x16, 10 tours, 5 shares
secureAESlike_16_10_5.o: secureAESlike.c secureAESlike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DSHARES=5 -c $< -o $@

# Compilation de la bibliothèque gérant les "shares" (nb de shares définis par la variable de Makefile)
shares.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) -c $< -o $@

# Compilation de la bibliothèque gérant les "shares" (nb de shares définis ici)
shares_1.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=1 -c $< -o $@

shares_3.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=3 -c $< -o $@

shares_5.o: shares.c shares.h
	$(CC) $(CFLAGS) -DSHARES=5 -c $< -o $@

# Conversion des sbox
sbox_tools.o: sbox_tools.c sbox_tools.h
	$(CC) $(CFLAGS) -c $< -o $@

# Création du programme de benchmark sur les mêmes entrées
# (testé sur amd64)
# nombre de shares définis par la variable de Makefile
bench_aesLikes: aesLike_16_10.o sbox_tools.o secureAESlike_16_10.o shares.o gf256.o bench_aesLikes.c
	$(CC) $(CFLAGS) $^ -o $@

# nombre de shares fixés
bench_aesLikes_shares3: aesLike_16_10.o sbox_tools.o secureAESlike_16_10_3.o shares_3.o gf256.o bench_aesLikes.c
	$(CC) $(CFLAGS) $^ -o $@

bench_aesLikes_shares1: aesLike_16_10.o sbox_tools.o secureAESlike_16_10_1.o shares_1.o gf256.o bench_aesLikes.c
	$(CC) $(CFLAGS) $^ -o $@

bench_aesLikes_shares5: aesLike_16_10.o sbox_tools.o secureAESlike_16_10_5.o shares_5.o gf256.o bench_aesLikes.c
	$(CC) $(CFLAGS) $^ -o $@

# Les tests de validité (pas franchement unitaires)
test_aesLike: aesLike_2_2.o sbox_tools.o gf256.o test_aesLike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 $^ -o $@

test_sbox_tools: sbox_tools.o gf256.o test_sbox_tools.c
	$(CC) $(CFLAGS) $^ -o $@

test_shares: shares.o gf256.o test_shares.c
	$(CC) $(CFLAGS) -DSHARES=$(SHARES) $^ -o $@

test_secureAESlike: shares_3.o gf256.o secureAESlike_2_2_3.o sbox_tools.o aesLike_2_2.o test_secureAESlike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=3 $^ -o $@

# exécution d'un programme défini par une règle, préfixé par run
# exemple : run_test_secureAESlike
run_%: %
	./$^

tests: test_aesLike test_sbox_tools test_shares test_secureAESlike

exec_tests: run_test_shares run_test_sbox_tools run_test_aesLike run_test_secureAESlike

# génération des tables pour gf256
genTables: gf256.o genTables.c
	$(CC) $(CFLAGS) $^ -o $@

# Version debug (messages de traçage activés, en fait)
aesLike_2_2_debug.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=1 -DDEBUG -c $< -o $@

aesLike_16_10_debug.o: aesLike.c aesLike.h
	$(CC) $(CFLAGS) -DLINEAR_SIZE=16 -DNB_ROUNDS=10 -DDEBUG -c $< -o $@

sbox_tools_debug.o: sbox_tools.c sbox_tools.h
	$(CC) $(CFLAGS) -DDEBUG_LAGRANGE -c $< -o $@

bench_aesLikes_debug: aesLike_16_10.o gf256.o sbox_tools.o bench_aesLikes.c
	$(CC) $(CFLAGS) -DDEBUG $^ -o $@

test_aesLike_debug: aesLike_2_2_debug.o gf256.o sbox_tools.o test_aesLike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=1 -DDEBUG $^ -o $@

test_secureAESlike_debug: shares_3.o gf256.o secureAESlike_2_2_3.o test_secureAESlike.c
	$(CC) $(CFLAGS) -DLINEAR_SIZE=2 -DNB_ROUNDS=2 -DSHARES=3 -DDEBUG $^ -o $@

# nettoyage du résultat
clean:	
	-rm -f *~ *.o {bench,test}_aesLike{_debug,,s,s-debug} test_sbox_tools{_debug,} genTables test_secureAESlike{_debug,} test_shares
