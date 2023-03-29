
#include <stdio.h>
#include <gmp.h>
#include <unistd.h>
#include "numtheory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "randstate.h"
#include "ss.h"
#include <time.h>
#include <sys/stat.h>

#define OPTIONS "i:o:n:hv"

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Encrypts data using SS encryption.\n"
                    "   Encrypted data is decrypted by the decrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to encrypt (default: stdin).\n"
                    "   -o outfile      Output file for encrypted data (default: stdout).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n");
    return;
}

int main(int argc, char **argv) {

    // Set default values for input and output files
    FILE *infile_h = stdin, *outfile_h = stdout, *pvfile_h;
    char *infile = NULL, *outfile = NULL, *pvfile = "ss.pub";
    bool verbose = false;

    int opt = 0;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "i:o:n:hv")) != -1) {

        switch (opt) {
        case 'i':
            // Open input file and set infile variable
            infile = optarg;
            break;
        case 'o':
            // Open output file and set outfile variable
            outfile = optarg;
            break;
        case 'n':
            // Set public key file
            pvfile = optarg;
            break;
        case 'v':
            // Set verbose flag
            verbose = true;
            break;
        case 'h':
            // Print help and usage and exit
            print_help();
            return 0;
        default:
            // Print help and usage and exit
            print_help();
            return 1;
        }
    }

    // Open public key file
    pvfile_h = fopen(pvfile, "r");

    // If an input file was specified, open it and set infile_h to point to it
    if (infile != NULL) {
        infile_h = fopen(infile, "r");
    }

    // If the input file does not exist, print an error message and exit
    if (infile_h == NULL) {
        printf("%s: No such file or directory\n", infile);
        exit(0);
    }

    // If an output file was specified, open it and set outfile_h to point to it
    if (outfile != NULL) {
        outfile_h = fopen(outfile, "w");
    }

    // Initialize a GMP integer and read the public key from the file
    mpz_t n;
    mpz_inits(n, NULL);
    char *username = getenv("USER");
    ss_read_pub(n, username, pvfile_h);

    // If the verbose flag is set, print some information about the public key
    if (verbose) {
        gmp_fprintf(stderr, "user: %s\n", username);
        gmp_fprintf(stderr, "n (%zu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(infile_h, outfile_h, n);

    // clear and return
    fclose(pvfile_h);
    fclose(infile_h);
    fclose(outfile_h);
    mpz_clears(n, NULL);

    return 0;
}
