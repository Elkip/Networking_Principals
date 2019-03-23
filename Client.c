#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "message.h"    /* for struct defs */
#include "functions.h"

#define PORT 27403

int main(int argc, char *argv[]) {
    
    struct sockaddr_in brokerAddr;        /* Echo server address */
    struct sockaddr_in fromAddr;          /* Source address of echo */
    socklen_t fromSize;                   /* In-out of address size for recvfrom() */
    struct P2Key reg, req;                //register an request structure
    struct Key2P broker_info;             //struct containing broker pub key
    struct B2C stock;                     //buy/sell request
    int stock_e;                          //Encyrpted struct
    int pub_key, priv_key;                //Keys
    unsigned short TCPport = PORT + 1;    /*TCP Port*/
    int TCPSock;                          /*TCP socket*/
    int trans, numStock, trans_id;        //user inputs
    int broker_key, msgLen;               //broker key and message length
    int stock_d;                          //decyrptred struct
    int structSize;                       //size of structure
    
    /* Test for correct number of arguments */
    if (argc != 1)  {
        DieWithError("Incorrect number of arguments");
    }

    /* Construct the server address structure */
    memset(&brokerAddr, 0, sizeof (struct sockaddr_in)); /* Zero out structure */
    brokerAddr.sin_family = AF_INET; /* Internet addr family */
    brokerAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Server IP address */ /* Server IP address */
    brokerAddr.sin_port = htons(TCPport); /* Server port */
    
    //generate a public key
    pub_key = KeyGen(&priv_key); 
    
    //fill out registry
    memset(&reg, 0, sizeof (struct P2Key));
    reg.request_type = regista;
    reg.principal_id = 1; //1 for client 2 for broker
    reg.public_key = pub_key;
    structSize = sizeof (struct P2Key);

    //register with Keymanager   
    printf("Registering client...\n");
    SetKey(reg);
    
    //request broker id
    memset(&req, 0, sizeof (struct P2Key));
    req.request_type = request_key;
    req.principal_id = 2;
    req.public_key = pub_key;
    printf("Sending key request...\n");
    broker_key = GetKey(req);
    printf("Done.\n\n\n");

    //User Input
    while (1) {
        printf("To buy stock enter '1'\nTo sell stock enter '2'\nTo Quit enter '0'\n");
        scanf("%i", &trans);

        switch (trans) {
            case 1:
                printf("Enter the number of stocks to purchase from 1 to 9:\n");
                scanf("%i", &numStock);
                break;
            case 2:
                printf("Enter the number of stocks to sell from 1 to 9:\n");
                scanf("%i", &numStock);
                break;
            case 0:
                exit(0);
            default:
                DieWithError("You didn't enter a valid selection, system will now hang");
        }

        //fill buy/sell request struct
        stock.client_id = 2;
        stock.num_stocks = numStock;
        stock.transaction_id = trans;
	stock.request_type = confirm; 
		
        //convert to a single int to encypt
        stock_d = StructToInt(stock);
        stock_e = Enc(stock_d, broker_key);
        
        /* Create a reliable, stream socket using TCP */
        if ((TCPSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("socket() failed");
        
        /* Establish the connection to the broker */
        printf("Establishing TCP connection with broker\n");
        if (connect(TCPSock, (struct sockaddr *) &brokerAddr, sizeof (brokerAddr)) < 0)
            DieWithError("connect() failed");
        printf("Sending Transaction...\n");

        /* Send buy request to broker */
        if (msgLen = send(TCPSock, &stock_e, sizeof (int), 0) < 0)
            DieWithError("Failed to send buy request");
        printf("Sent.\n");

        /* Recv a response */
        if (msgLen = recv(TCPSock, &stock_e, sizeof (int), 0) < 0)
            DieWithError("Client failed to recieve verification");
        stock_d = Dec(stock_e, priv_key);
        printf("Response received.\n");

        // Send verify message to broker 
	printf("Verifying message...\n");
        stock_e = Enc(stock_d, priv_key);
        if (msgLen = send(TCPSock, &stock_e, sizeof (int), 0) < 0)
                DieWithError("Failed to send verification");
        
        //receive done response
        if (msgLen = recv(TCPSock, &stock_e, sizeof (int), 0) < 0)
            DieWithError("Client failed to recieve verification");
        
        //Decrypt done response
	printf("Verification message recieved\n");
        stock_d = Dec(stock_e, priv_key);
	IntToStruct(&stock,stock_d);
		
	if (stock.request_type==done)
		printf("You traded %d shares of stock\n", stock.num_stocks);
		
        printf("Success. \n");
        
    }
    exit(0);
}
