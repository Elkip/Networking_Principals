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

#define PORT 27402

void SetKey(struct P2Key reg) {
    
    struct sockaddr_in keymanageAddr;   //Keyamanager Address
    int portno = PORT;                  //port number
    int sock;                           //UDP Socket
    
    //configure local address structure for the key manger
    memset(&keymanageAddr, 0, sizeof (struct sockaddr_in)); /* Zero out structure */
    keymanageAddr.sin_family = AF_INET; /* Internet addr family */
    keymanageAddr.sin_port = htons(portno); /* Server port */
    keymanageAddr.sin_addr.s_addr = inet_addr("127.0.0.2"); /* Keymanager IP address */

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("UDP socket() failed");
    
    //Register principal
    if (sendto(sock, &reg, sizeof (struct P2Key), 0,
            (struct sockaddr *) &keymanageAddr, sizeof (struct sockaddr_in)) < 0)
        DieWithError("Registration failed");
    printf("Done.\n");

    close(sock);
}