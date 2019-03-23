/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);
long sma( long x,  long H,  long n);

long KeyGen(long *priv) {

    long p = 47, q = 31;    //p and q are prime
    long e = 27;    //e stands for encrypt
    long totient = (p - 1)*(q - 1);

    //e must be smaller than and co-prime to the totient
    while (e < totient) {
        if (gcd(e, totient) == 1)
            break;
        else
            e++;
    }

    // Private key (d stands for decrypt)
    // d*e = 1 % mod totient 
    *priv = sma(e,-1,totient); //store private key

    printf("Public key: %ld \n", e);
    printf("Private Key: %ld \n", *priv);
    return e;
}

int gcd(int a, int b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a%b);
}
