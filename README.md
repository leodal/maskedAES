Higher Order Masking for AES-like variations
======

We provide a C implementation of an anti-SCARE countermeasure of an "AES-like" cryptosystem. We call an "AES-like" a simple cipher with a linear part (i.e. a matix product) that applies to a vector which is xored to round-keys and passed throught an Sbox.

We apply a masking counter-mesure on each element, either in the linear part (Rivain-Proof) and in the sbox. Since our implementation is generic to all Sbox, we apply a Carlet et al. counter-mesure.

What is implemented
-------------------

* AES-like without countermeasure
* AES-like with countermeasure

Files
-----
* gf256.h and gf256.c : arithmetic operations in the AES field
* genTables.c : generation of the tables used in gf256.{c,h}
* aesLike.h and aesLike.c : naive implementation of the unprotected cryptosystem
* secureAESlike.h and secureAESlike.c : implementation if the cryptosystem protecting either linear part and Sbox
* shares.c and shares.h : operations on protected values
* sbox_tools.h and sbox_tools.c : conversion operations for sbox for Carlet et. al. counter-mesure
* test_*.c : tests for the above files

How to use
----------
The size of the cryptosystem, the number of round and the number of shares are defined during compilation by the respective constants LINEAR_SIZE, NB_ROUNDS and SHARES.

Run all tests : make exec_tests
Run benchmark : make bench_aesLikes (run a time mesure for unprotected and protected cryptosystem)

Notes
----
* We do not claim that in practice the implementation would be secure against a real-world attack. The implementation is only provided for illustrative purpose, and timing comparisons.
