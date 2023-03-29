#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

// header
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS "b:i:n:d:s:vh"

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Generates an SS public/private key pair.\n"
                    "\n"
                    "USAGE\n"
                    "   ./keygen [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -b bits         Minimum bits needed for public key n (default: 256).\n"
                    "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n"
                    "   -d pvfile       Private key file (default: ss.priv).\n"
                    "   -s seed         Random seed for testing.\n");
    return;
}

int main(int argc, char **argv) {

    uint64_t nbits = 256; // default bits
    uint64_t iters = 50; // default iterations
    FILE *pbfile_h, *pvfile_h;

    char *pbfile = "ss.pub", *pvfile = "ss.priv";
    uint64_t seed = time(NULL);
    bool verbose = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'h':
            print_help();
            return 1;
            break;
        case 'v': verbose = true; break;
        case 'b': nbits = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'n': pbfile = optarg; break;
        case 'd': pvfile = optarg; break;
        case 's':
            seed = (uint64_t) strtoul(optarg, NULL, 10);
            break;
            //  seed_check = true;
        default:
            print_help();
            return 1;
            break;
        }
    }

    // make sure that the bit amount is < than 0
    // make sure that the bit amount is positive
    if (nbits <= 0) {
        exit(0);
    }

    // open public key file for writing
    pbfile_h = fopen(pbfile, "w+");
    if (!pbfile_h) {
        perror("Error opening public key file");
        return 1;
    }

    // open private key file for writing
    pvfile_h = fopen(pvfile, "w+");
    if (!pvfile_h) {
        perror("Error opening private key file");
        return 1;
    }

    // get username
    char *username = getenv("USER"); // get username

    // establish file permissions
    uint64_t perm = fileno(pvfile_h); // variable to pass into fchmod to establish permissions
    fchmod(perm, 0600);

    randstate_init(seed);

    // initialize  variables for makepub and makepriv
    mpz_t p, q, n, pq, d, s;
    mpz_inits(p, q, n, pq, d, s, NULL);

    ss_make_pub(p, q, n, nbits, iters); // make public key
    ss_make_priv(d, pq, p, q); //  m ake private key

    // write to the files
    ss_write_pub(n, username, pbfile_h); // write keys to their files
    ss_write_priv(pq, d, pvfile_h); // write keys to their files

    if (verbose) {
        gmp_fprintf(stderr, "user = %s\n", username); //  username
        gmp_fprintf(stderr, "p  (%lu bits) = %Zu\n", mpz_sizeinbase(p, 2),
            p); // p -> first large prime p ->
        gmp_fprintf(
            stderr, "q  (%lu bits) = %Zu\n", mpz_sizeinbase(q, 2), q); // q -> second large prime q
        gmp_fprintf(stderr, "n  (%lu bits) = %Zu\n", mpz_sizeinbase(n, 2), n); // n -> public key n
        gmp_fprintf(
            stderr, "pq (%lu bits) = %Zu\n", mpz_sizeinbase(pq, 2), pq); // pq -> the signatures
        gmp_fprintf(
            stderr, "d  (%lu bits) = %Zu\n", mpz_sizeinbase(d, 2), d); // d -> private exponent d
    }

    fclose(pbfile_h);
    fclose(pvfile_h);
    randstate_clear();
    mpz_clears(p, q, n, pq, d, s, NULL);

    return 0;
}
