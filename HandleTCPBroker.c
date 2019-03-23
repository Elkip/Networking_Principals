#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for exit() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include "message.h" 

void HandleTCPBroker(int clntSocket, int client_key, int pub_key, int priv_key)
{

    struct B2C req;                    //Principle to principle 
    int stock_e;                    //Encrypted stock info
    int stock_d;                       //Decrypted trade info
    int msgLen;                        //Size of received message

    /* Receive message from client TCP */
    if ((msgLen = recv(clntSocket, &stock_e, sizeof (int), 0)) < 0)
        DieWithError("recv() failed");
    stock_d = Dec(stock_e, priv_key);
    printf("Recieved message from client\n");

    //Here is where the program would actually do the stock trading stuff
    //But that's not the point of this assignment
    printf("Trading...");

    //send back Encrypted trade with verification
    printf("Sending verifcation\n");
    stock_e = Enc(stock_d, client_key);
    if (msgLen = send(clntSocket, &stock_e, sizeof (int), 0) < 0)
        DieWithError("Failed to send verification");
    printf("Sent.\n");
    
    //receive verification
    printf("Waiting for verification...\n");
    if ((msgLen = recv(clntSocket, &stock_e, sizeof (int), 0)) < 0)
         DieWithError("recv() failed");
    stock_d = Dec(stock_e, client_key);
    if (stock_e > 999)
        DieWithError("Verification failed");
    printf("Verification Recieved.\n");
    
    //send done signal
    printf("Trade successful, sending done key\n");
	stock_d += 1000;
    stock_e = Enc(stock_d, client_key);
    if (msgLen = send(clntSocket, &stock_e, sizeof (int), 0) < 0)
        DieWithError("Failed to send done message");
    printf("Sent. Done\n");
    
}
