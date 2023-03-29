// !!!!!!!!!!!!!!!!!!!!! THIS IS FOR TESTING !!!!!!!!!!!!!!!!!!!!!!!!!!! //
// XXXXXXXXXXXXXXXXXXXXX GCD XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX //
/*
int main(void) {
    mpz_t a, b, g;
    mpz_init(a);
    mpz_init(b);
    mpz_init(g);

    // Test case 1
    mpz_set_ui(a, 35);
    mpz_set_ui(b, 15);
    gcd(g, a, b);
    printf("Test case 1: GCD of 35 and 15 is ");
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // Test case 2
    mpz_set_ui(a, 45);
    mpz_set_ui(b, 20);
    gcd(g, a, b);
    printf("Test case 2: GCD of 45 and 20 is ");
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // Test case 3
    mpz_set_ui(a, 50);
    mpz_set_ui(b, 30);
    gcd(g, a, b);
    printf("Test case 3: GCD of 50 and 30 is ");
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // clear the mpz_ts
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(g);
    return 0;
}*/

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx POW_MOD xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx //

/*int main(void) {
    mpz_t a, b, c, g;
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);
    
    mpz_init(g);

    // Test case 1
    int a1 = 4;
    int b1 = 12;
    int c1 = 3;
    
    mpz_set_ui(a, a1);
    mpz_set_ui(b, b1);
    mpz_set_ui(c, c1);
    
    pow_mod(g, a, b, c);
    printf("Test case 1: %d and %d mod %d  is ", a1, b1, c1);
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // Test case 2
    int a2 = 2;
    int b2 = 3;
    int c2 = 5;
    
    mpz_set_ui(a, a2);
    mpz_set_ui(b, b2);
    mpz_set_ui(c, c2);
    
    pow_mod(g, a, b, c);
    printf("Test case 2: %d and %d mod %d is ", a2, b2, c2);
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // Test case 3
    int a3 = 2;
    int b3 = 5;
    int c3 = 13;
    
    mpz_set_ui(a, a3);
    mpz_set_ui(b, b3);
    mpz_set_ui(c, c3);
    
    pow_mod(g, a, b, c);
    printf("Test case 3: %d and %d mod %d is ", a3, b3, c3 );
    mpz_out_str(stdout, 10, g);
    printf("\n");

    // clear the mpz_ts
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(g);
    return 0;
}*/

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IS_PRIME xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx //
/*
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <assert.h>

// header files
#include "numtheory.h"
#include "randstate.h"

int main(void) {
    //mpz_t a, b, g;
    mpz_t a, g;
    //mpz_t a;
    mpz_init(a);
    //mpz_init(b);
    mpz_init(g);

    // Test case 1
    uint64_t a1 = 1000; // k or the loop amount 
    //mpz_set_ui(g, 2);  // n  
    
    //mpz_set_ui(a, a1);
    //mpz_set_ui(b, 15);
    randstate_init(100);
    is_prime(g, a1);
        
   uint64_t count = 0;
   printf("The prime numbers in between the range 1 to %lu:\n",a1);
   for(uint64_t num = 1; num <= a1 ;num++){
      count = 0;
      for(uint64_t i=2; i<=num/2 ;i++){
         if(num % i == 0){
            count++;
         break;
      }
   }
   if(count == 0 && num != 1){
      mpz_set_ui(g, num);
      printf("case: %lu = %d\n", num, is_prime(g, a1));
      //printf("%lu \n",num);
      
      }
   }
       
    //printf("Test case 1: %d", is_prime(g, a1));
    randstate_clear();
    //mpz_out_str(stdout, 10, g);
    //printf("\n");

    
    // clear the mpz_ts
    mpz_clear(a);
    //mpz_clear(b);
    mpz_clear(g);
    return 0;
}*/

/*
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <assert.h>

// header files
#include "numtheory.h"
#include "randstate.h"

int main(void) {
    //mpz_t a, b, g;
    mpz_t a, g;
    //mpz_t a;
    mpz_init(a);
    //mpz_init(b);
    mpz_init(g);

    // Test case 1
    uint64_t a1 = 1000; // k or the loop amount 
    //mpz_set_ui(g, 2);  // n  
    
    //mpz_set_ui(a, a1);
    
    uint64_t input = 2;
    
    mpz_set_ui(g, input); // change n or the input
    
    
    randstate_init(100);
    is_prime(g, a1);
    printf("Test case 1: %lu = %d\n", input, is_prime(g, a1));
      
       
    //printf("Test case 1: %d", is_prime(g, a1));
    randstate_clear();
    //mpz_out_str(stdout, 10, g);
   // printf("\n");

    
    // clear the mpz_ts
    mpz_clear(a);
    //mpz_clear(b);
    mpz_clear(g);
    return 0;
}*/

// xxxxxxxxxxxxxxxxxxxxxxxxxxxx MAKE_PRIME xxxxxxxxxxxxxxxxxxxxxxx//
/*
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// header files
#include "numtheory.h"
#include "randstate.h"

#include <assert.h>

int main(void) {
    
    // Generate a random prime number with 256 bits and 50 iterations
    mpz_t p; 
    mpz_init(p);
    
    randstate_init(100);
    uint64_t iters = 50;
    uint64_t bits = 20;
    
    for(uint64_t seed = 2; seed < 30; seed++){
      make_prime(p, bits, iters);
      gmp_printf("-> %Zd\n", p);
    }

    //gmp_printf("Random prime number: %lu\n", p);
    randstate_clear();
    
    mpz_clear(p);
    return 0;
}*/
