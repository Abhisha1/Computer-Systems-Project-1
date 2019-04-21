/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program deals with the parsing of http requests
 * */
#include <stdbool.h>
#include "hashtable.h"

// Supported HTTP Request Methods
typedef enum
{
    GET,
    POST,
    UNKNOWN
} METHOD;

// HTTP request structure
typedef struct Request {
    METHOD method;
    char *url;
    char *version;
    HashTable *header;
    char *body;
} Request;


// Parses the request and stores request information
Request* parse_request(char* request_message);

// Deallocates memory
void free_request(Request* req);

char* insert_headers(char* request_message, Request * req);