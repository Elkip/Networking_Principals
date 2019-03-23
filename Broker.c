#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>
#include <sys/types.h> //definitions for data types in system calls
#include <sys/socket.h> /* for recv() and send() */
#include <netinet/in.h> //Constants and structures for internet domain addresses
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "message.h" /* for struct defs */
#include "functions.h"

#define PORT 27403
#define MAXPENDING 5    /* Maximum outstanding connection requests */

void HandleTCPBroker(int clntSocket, int client_key, int pub_key, int priv_key);

int main(int argc, char *argv[]) {
    struct sockaddr_in broker_addr;
    struct sockaddr_in clntAddr;        //addresses
    struct P2Key reg, req;              //register and request structure
    struct B2C trade;                   //Principle to princple connection
    int pub_key, priv_key;              //Keys
    int portno = PORT + 1;                  //portnumber
    int client_key;                     //client key 
    int TCPSockS, TCPSockC;    //sockets for client server TCP connection
    socklen_t cliAddrLen;               //recived message size 
    
    //check to make sure port number was included in argument
    if (argc != 1) {
        DieWithError("Input Incorrect");
    }
    
    //configure local address structure for the server
    memset(&broker_addr, 0, sizeof (struct sockaddr_in)); /* Zero out structure */
    broker_addr.sin_family = AF_INET; /* Internet addr family */
    broker_addr.sin_port = htons(portno); /* Server port */
    broker_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Server IP address */

    //generate a public key
    pub_key = KeyGen(&priv_key);

    //fill out registry struct
    memset(&reg, 0, sizeof (struct P2Key));
    reg.request_type = regista;
    reg.principal_id = 2;
    reg.public_key = pub_key;

    //register with Keymanager
    printf("Registering Broker...");
    SetKey(reg);
    
    //Create TCP port
    printf("Creating TCP port for incoming clients...\n");
    /* Create socket for incoming connections */
    if ((TCPSockS = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("TCP socket() failed");
    /* Bind to the local address */
    if (bind(TCPSockS, (struct sockaddr *) &broker_addr, sizeof (broker_addr)) < 0)
        DieWithError("bind() failed");
    /* Mark the socket so it will listen for incoming connections */
    if (listen(TCPSockS, MAXPENDING) < 0)
        DieWithError("listen() failed");
    printf("Done.\nReady for input\n");

    //run forever
    for (;;) {
        //set address length
        cliAddrLen = sizeof (struct sockaddr_in);
        
        /* wait for client to connect */
	printf("Ready for client request...");
        if ((TCPSockC = accept(TCPSockS, (struct sockaddr *) &clntAddr,
                &cliAddrLen)) < 0)
            DieWithError("accept() failed");
        printf("Handling client %s\n", inet_ntoa(clntAddr.sin_addr));
        
        //get client public key
        req.principal_id = 1;  //1 for client 2 for broker
        req.request_type = request_key;
        req.public_key = pub_key;
        client_key = GetKey(req);
        
        HandleTCPBroker(TCPSockC, client_key, pub_key, priv_key);
    }

    //this line should never be reached
    return 0;
}

