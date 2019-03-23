#include <stdio.h>
#include "message.h"

//The purpose of this function is to turn the decrypted int back to a struct
void IntToStruct(struct B2C *b, int i) {
    
	if (i > 999) {
        b->request_type = done;
        i=i%1000;
    }
    else
        b->request_type = confirm;

    //Store integers by doing basic int devision
    b->client_id = i/100;
    i = i%100;
    b->num_stocks = i/10;
    i= i%10;
    b->transaction_id = i;
    
    printf("Output Of Int to struct: %i %i %i \n", b->client_id, b->num_stocks, b->transaction_id);
}