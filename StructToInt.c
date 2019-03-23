#include <stdio.h>
#include "message.h"

//The purpose of this function is to turn the message struct into an int to be encrypted
int StructToInt(struct B2C b) {
    int total;
    if (b.request_type == done)
        total+=1000;
	
    //Assuming these are all SINGLE DIGIT NUMBERS 
    total += b.client_id*100;
    total += b.num_stocks*10;
    total += b.transaction_id;
    
    printf("Output Of Struct to int: %i\n", total);
    return total;
}
