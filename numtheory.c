#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// header files
#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    /*
 * GCD(a,b)
 *  while b̸ = 0 
 *   t   ←b
 *   b   ←a mod b
 *   a   ←t 
 *  return a
 */
    mpz_t t, temp_b, temp_a;
    mpz_inits(t, temp_b, temp_a, NULL);

    mpz_set(temp_b, b);
    mpz_set(temp_a, a);

    while (mpz_cmp_ui(temp_b, 0) != 0) {
        mpz_set(t, temp_b);
        mpz_mod(temp_b, temp_a, temp_b); // loop through until b is equal to 0
        mpz_set(temp_a, t);
    }
    mpz_set(g, temp_a); // final output
    // clear the mpz_ts
    mpz_clears(t, temp_b, temp_a, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    /*
 * MOD-INVERSE(a,n)
 *   (r,r′) ← (n,a) 
 *   (t,t′) ← (0,1)
 *   while r′̸=0
 *     q←⌊r/r′⌋ 
 *     (r,r′)←(r′,r−q×r′) 
 *     (t,t′)←(t′,t−q×t′)
 *   if r > 1
 *     return no inverse
 *   if t < 0 
 *     t←t+n
 *   return t
 */

    mpz_t r, t, rp, tp, tmp_r, tmp_t, q;

    mpz_init_set(r, n);
    mpz_init_set(rp, a); // setting r & r_prime

    mpz_init_set_ui(t, 0);
    mpz_init_set_ui(tp, 1); // setting t & t_prime

    mpz_inits(tmp_r, tmp_t, q, NULL);

    while (mpz_cmp_ui(rp, 0)) { // r 1= 0
        mpz_fdiv_q(q, r, rp); // q←⌊r/r′⌋

        // (r,r′)←(r′,r−q×r′)
        mpz_set(tmp_r, r);
        mpz_set(r, rp);
        mpz_mul(rp, q, rp);
        mpz_sub(rp, tmp_r, rp);

        // (t,t′)←(t′,t−q×t′)
        mpz_set(tmp_t, t);
        mpz_set(t, tp);
        mpz_mul(tp, q, tp);
        mpz_sub(tp, tmp_t, tp);
    }

    // return no inverse if r is greater than 1
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(t, 0);
    }

    // increments t by n if its less than 0
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }

    mpz_set(o, t);

    // clear the mpz
    mpz_clears(r, t, rp, tp, tmp_r, tmp_t, q, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    /*
 * Function: power_mod
 * -------------------
 * POWER-MOD(a,d,n)
 *   v←1 p←a 
 *   while (d > 0)S
 *     if ODD(d)
 *       v←(v×p) mod n
 *     p←(p×p) mod n
 *     d ← ⌊d/2⌋ 
 *   return v
 */

    mpz_t v, p, tmp_d;
    // set v = 1 and p = a
    mpz_init_set_ui(v, 1);
    mpz_init_set(p, a);

    mpz_init_set(tmp_d, d);

    while (mpz_cmp_ui(tmp_d, 0) > 0) {
        if (mpz_odd_p(tmp_d)) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }

        mpz_mul(p, p, p);
        mpz_mod(p, p, n);

        mpz_fdiv_q_ui(tmp_d, tmp_d, 2); // floor division
    }

    // return the value
    mpz_set(o, v);
    // clear the mpz's
    mpz_clears(v, p, tmp_d, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    /*
 * MILLER-RABIN(n,k)
 *   write n−1 = 2^s r such that r is odd 
 *   for i←1 to k
 *       choose random a ∈{2,3,...,n−2} 
 *       y = POWER-MOD(a,r,n) 
 *       if y̸=1 and y̸=n−1
 *           j←1 
 *           while j≤ s−1 and y̸=n−1
 *               y ← POWER-MOD(y,2,n) 
 *               if y==1
 *                   return FALSE 
 *               j←j+1
 *         if  y̸= n−1 
 *           return FALSE
 *   return TRUE
 */
    if (mpz_cmp_ui(n, 2) < 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 2) == 0) {
        return true;
    }
    if (mpz_even_p(n) != 0) {
        return false;
    }

    // temp mpz_ts declared and initialized here so as not to change the values of the original parameters
    mpz_t dividend, r, two, s, div, y, a, s_minus, n_minus, j, result;
    mpz_inits(dividend, r, two, s, div, y, a, s_minus, n_minus, j, result, NULL);

    // [0, n)
    // [2, n+2)
    // [0, n-4) + 2

    mpz_sub_ui(dividend, n, 1);

    mpz_set_ui(div, 1);

    mpz_sub_ui(result, n, 4);

    mpz_sub_ui(n_minus, n, 1); // temp_n

    // while loop used to find s and r by repeatedly dividing n-1 by 2 until r is odd
    while (mpz_odd_p(r) == 0) {
        mpz_div(r, n_minus, div);
        mpz_mul_ui(div, div, 2);
        mpz_add_ui(s, s, 1); // increment by 1 each loop
    }

    mpz_sub_ui(s_minus, s, 1);

    for (uint64_t i = 1; i < iters; i++) {

        mpz_urandomm(
            a, state, result); // calls u_randomm which generates numbers from 0 to n-1 inclusive
        mpz_add_ui(a, a, 2); // increments a by 2 in order to set the intverval from 2 to n-2

        pow_mod(y, a, r, n); // calls pow_mod to set y to a^r(mod n)

        if (mpz_cmp_ui(y, 1) != 0
            && mpz_cmp(y, n_minus) != 0) { // conditional to check if y isn't 1 and y isn't n-1
            mpz_set_ui(j, 1); // sets j to 1

            while (mpz_cmp(j, s_minus) <= 0 && mpz_cmp(y, n_minus) != 0) {

                mpz_set_ui(two, 2);
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {

                    mpz_clears(dividend, r, two, s, div, y, a, s_minus, n_minus, j, result, NULL);
                    return false;
                    // returns false if y is equal to 1
                }
                mpz_add_ui(j, j, 1);
                // increments j by 1
            }
            if (mpz_cmp(y, n_minus) != 0) {
                mpz_clears(dividend, r, two, s, div, y, a, s_minus, n_minus, j, result, NULL);
                return false;
                // returns false if y is not equal to n-1
            }
        }
    }

    mpz_clears(dividend, r, two, s, div, y, a, s_minus, n_minus, j, result, NULL);
    return true;
}

// Function that generates a random prime number with a given
// number of bits, using the Miller-Rabin primality test
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    do {
        mpz_urandomb(p, state, bits);

    } while (is_prime(p, iters) == false);
}
