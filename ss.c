#include <stdio.h>
#include <gmp.h>
#include <unistd.h>
#include "numtheory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "randstate.h"
#include "ss.h"
#include <time.h>
#include <sys/stat.h>

extern gmp_randstate_t state;

//
//  Generates the components for a new SS key.
//
//  Provides:
//  p: first prime
//  q: second prime
//  n: public modulus/exponent
//
//  Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {

    // creating p and q
    // [nbits/5, (2 × nbits)/5)
    uint64_t lower_bound = nbits / 5;
    uint64_t upper_bound = (2 * nbits) / 5;

    uint64_t pbits = lower_bound + random() % (upper_bound - lower_bound);

    make_prime(p, pbits, iters); // make first prime p

    uint64_t qbits = nbits - pbits - pbits; // n - p

    make_prime(q, qbits, iters); // make second prime p

    //creating n
    mpz_t n_v;
    mpz_init(n_v);
    mpz_mul(n_v, p, p); // n = p * q
    mpz_mul(n, n_v, q); // n = p * p * q

    mpz_clear(n_v);
}

//
// Generates components for a new SS private key.
//
// Provides:
//   d: private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    // restrict to 𝑁 product of distinct primes; for two such primes
    //  𝜑(𝑁)=(𝑝−1)⋅(𝑞−1), 𝜆(𝑁)=lcm(𝑝−1,𝑞−1), and 𝜑(𝑁)=𝜆(𝑁) ⋅ gcd(𝑝−1,𝑞−1).
    mpz_t p_minus_1, q_minus_1, lcm, p_times_p, g, total, n;
    mpz_inits(p_minus_1, q_minus_1, lcm, p_times_p, g, total, n, NULL);

    // calculate p-1 and q-1
    mpz_sub_ui(p_minus_1, p, 1); // p - 1
    mpz_sub_ui(q_minus_1, q, 1); // q -1

    // get the private key by getting the lcm(p-1, q-1)
    gcd(g, p_minus_1, q_minus_1); // gcd((p-1), (q-1)) -> g = (p-1), (q-1)
    mpz_mul(total, p_minus_1, q_minus_1); // total = (p-1) * (q-1)
    mpz_fdiv_q(lcm, total, g);

    // calculate n
    mpz_mul(pq, p, q); // pq = p * q

    mpz_mul(p_times_p, p, p); // pp = p * p

    mpz_mul(n, p_times_p, q); // n = pp * q

    // using n and lcm in mod inverse
    mod_inverse(d, n, lcm);

    mpz_clears(p_minus_1, q_minus_1, lcm, p_times_p, g, total, n, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
}

//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    // E(m)=c=m^n (mod n).
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {

    mpz_t m, c;
    uint8_t *block;
    uint64_t k;
    mpz_init(m), mpz_init(c);

    size_t bytes_read;

    mpz_t n_squared;
    mpz_init(n_squared);

    mpz_sqrt(n_squared, n); // compute the square of n and store the result in n_squared
    k = (mpz_sizeinbase(n_squared, 2) - 1) / 8;

    // initialize the planintext ciphertext
    block = (uint8_t *) calloc(k, sizeof(uint8_t));

    block[0] = 0xFF; // declaration of array with prefix 0xFF

    while ((bytes_read = fread(block + 1, sizeof(uint8_t), k - 1, infile)) > 0) {

        // check if these is still bytes to read
        // covert the elements of the block to m
        mpz_import(m, bytes_read + 1, 1, sizeof(uint8_t), 1, 0, block);

        ss_encrypt(c, m, n); // call ss to get c

        gmp_fprintf(outfile, "%Zx\n", c); // print to the outfile
    }

    mpz_clears(m, c, n_squared, NULL);

    free(block);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    // D(c)= m = cd (mod pq)
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t m, c;
    uint8_t *block;
    uint64_t k;
    size_t bytes_read;
    mpz_init(m), mpz_init(c);

    //k = (log2(mpz_get_ui(n))-1)/8;
    k = (mpz_sizeinbase(pq, 2) - 1) / 8;

    // initialize the plaintext and ciphertext
    block = malloc(k * sizeof(uint8_t));

    block[0] = 0xFF; // declaration of array with prefix 0xFF

    while (gmp_fscanf(infile, "%Zx \n", c) != EOF) {

        ss_decrypt(m, c, d, pq);
        mpz_export(block, &bytes_read, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(block + 1, sizeof(uint8_t), bytes_read - 1, outfile);
    }

    free(block);
    mpz_clear(m);
    mpz_clear(c);
}
