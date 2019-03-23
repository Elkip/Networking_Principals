/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>

long sma( long x,  long H,  long n);

//Decryption
//m = (c ^d ) % n
long Dec(long msg, long d) {
    printf("Decrypting: %ld \n", msg);
    long p = 47, q = 31;
    long n = p*q;
    
    long m = sma(msg,d,n);

    printf("Decrypted Key: %ld \n", m);
    return m;
}
