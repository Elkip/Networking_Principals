/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>

long sma( long x,  long H,  long n);

long Enc(long msg, long e){
    printf("Encrypting: %ld \n", msg);
    long p = 47, q = 31;
    long n = p*q;
    
    long enc = sma(msg,e,n);
    printf("Encrypted Key: %ld \n", enc);
    return enc;
}

