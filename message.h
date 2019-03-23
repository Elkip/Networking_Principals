#ifndef MESSAGE_H 
#define MESSAGE_H

struct P2Key; //Client/Broker to Keymanage
struct Key2P; //Keymanage to Broker/Client
struct B2C; //Broker to Client and vice versea

typedef struct P2Key {

    enum {
        regista, request_key
    } request_type; /* same size as an unsigned int */
    unsigned int principal_id; /* client or broker identifier */
    int public_key; /* client’s or broker’s RSA public key */
} P2Key; /* an unsigned int is 32 bits = 4 bytes */

typedef struct Key2P {
    unsigned int principal_id; /* client or broker identifier */
    int public_key; /* client’s or broker’s RSA public key */
} Key2P; /* an unsigned short is 16 bits = 2 bytes */

typedef struct B2C {

    enum {
        confirm, done
    } request_type; /* same size as an unsigned int */
    unsigned int client_id; /* client identifier */
    unsigned int transaction_id; /* transaction identifier */
    unsigned int num_stocks; /* number of stocks */
} B2C;


#endif // MESSAGE_H
