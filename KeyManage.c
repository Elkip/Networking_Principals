#include <stdio.h> //for printf and fprintf
#include <arpa/inet.h>
#include <sys/socket.h> //for recv and send
#include <unistd.h> //for close
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "functions.h"

#define PORT 27402

int main(int argc, char *argv[]) {
    FILE *pFile; //File to store keys
    struct P2Key res, req; //result and request key structures
    struct Key2P send_key;
    struct sockaddr_in key_addr, fromAddr;
    int sock; //socket descriptor
    int port = PORT;
    int msgLen;
    socklen_t addrLen;

    if (argc != 1) {
        DieWithError("Invalid Number of arguments");
    }

    // Filling server information 
    memset(&key_addr, 0, sizeof (struct sockaddr_in));
    key_addr.sin_family = AF_INET;
    key_addr.sin_addr.s_addr = inet_addr("127.0.0.2");
    key_addr.sin_port = htons(port);

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    // Bind the socket to the local address 
    if (bind(sock, (const struct sockaddr *) &key_addr,
            sizeof (struct sockaddr_in)) < 0)
        DieWithError("bind() failed");
    printf("Ready to recieve key request\n");
    
    //run forever
    for (;;) {
        memset(&req, 0, sizeof (struct P2Key));
        memset(&fromAddr, 0, sizeof (struct sockaddr_in));
        addrLen = sizeof (struct sockaddr_in);
        //recieve message
        if ((msgLen = (recvfrom(sock, &req, sizeof (struct P2Key), 0,
                (struct sockaddr *) &fromAddr, &addrLen))) < 0)
            DieWithError("keymanager recvfrom() failed");
        printf("Handling client %s\n", inet_ntoa(fromAddr.sin_addr));
        
        
        
        //if the principal is requesting to registering, store it in a file
        if (req.request_type == regista) {
            printf("Writing key to file...\n");
            pFile = fopen("crypt.txt", "ab");
            fwrite(&req,sizeof(struct P2Key),1, pFile);
            printf("Written: %u %i ", req.principal_id, req.public_key);
            fclose(pFile);
            printf("Done\n");
        }
        
        
        
        //if pincipal is requesting key
        if (req.request_type == request_key) {
            pFile = fopen("crypt.txt", "rb");
            printf("Key requested for: %u %i ", req.principal_id, req.public_key);
            printf("Searching for Requested Key\n");
           
            while ( fread(&res,sizeof(struct P2Key),1, pFile) == 1 ) {
                printf("Read: %u %i\n", res.principal_id, res.public_key);
                printf("%ld\n", ftell(pFile));
                
                //if this is the client requested, send the key
                if (req.principal_id == res.principal_id) {
                    send_key.principal_id = req.principal_id;
                    send_key.public_key = req.public_key;
                    printf("Sending Key...\n");
                    if (sendto(sock, &send_key, sizeof (send_key), 0,
                            (struct sockaddr *) &fromAddr, sizeof (struct sockaddr_in)) != sizeof (send_key))
                        DieWithError("Could not send requested key");
                    printf("Requested key sent\nDone\n");
                    break;
                }

            }
                fclose(pFile);
                printf("End of file reached.\n");
                printf("Ready to recieve key request\n");
        }
    }
    
    //should never be reached
    return 0;
}