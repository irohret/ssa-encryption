#include <stdio.h>
#include <gmp.h> // Include the GNU Multiple Precision Arithmetic Library
#include <unistd.h>
#include "numtheory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "randstate.h"
#include "ss.h"
#include <time.h>
#include <sys/stat.h>

#define OPTIONS "i:o:n:hv" // Define the command-line options

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decrypts data using SS decryption.\n"
                    "   Encrypted data is encrypted by the encrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./decrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to decrypt (default: stdin).\n"
                    "   -o outfile      Output file for decrypted data (default: stdout).\n"
                    "   -n pvfile       Private key file (default: ss.priv).\n");
    return;
}

int main(int argc, char **argv) {

    FILE *infile_h = stdin, *outfile_h = stdout,
         *pvfile_h; // Initialize input and output file handlers
    char *infile = NULL, *outfile = NULL, *pvfile = "ss.priv"; // Initialize file names
    bool verbose = false; // Initialize verbose flag

    int opt = 0;

    // Parse command-line options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        switch (opt) {
        case 'i': infile = optarg; break;
        case 'o': outfile = optarg; break;
        case 'n': pvfile = optarg; break;
        case 'v': verbose = true; break;
        case 'h': print_help(); return 0;
        default: print_help(); return 1;
        }
    }

    // Open the private key file for reading
    pvfile_h = fopen(pvfile, "r");

    // If an input file was specified, open it for reading
    if (infile != NULL) {
        infile_h = fopen(infile, "r");
    }

    // If an output file was specified, open it for writing
    if (outfile != NULL) {
        outfile_h = fopen(outfile, "w");
    }

    // Initialize two big integers
    mpz_t pq, d;
    mpz_inits(d, pq, NULL);

    // Read the private key from the file
    ss_read_priv(pq, d, pvfile_h);

    // If the verbose flag is set, print the values of pq and d
    if (verbose) {
        gmp_fprintf(stderr, "pq (%lu bits) = %Zu\n", mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stderr, "d  (%lu bits) = %Zu\n", mpz_sizeinbase(d, 2), d);
    }

    // Decrypt the input file using the private key and write the result to the output file
    ss_decrypt_file(infile_h, outfile_h, d, pq);

    // Close all open file handlers and clear the big integers
    fclose(pvfile_h);
    fclose(infile_h);
    fclose(outfile_h);
    mpz_clears(d, pq, NULL);

    return 0;
}
