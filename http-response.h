/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program deals with all HTTP responses sent.
 * */
#include <stdbool.h>
#include "hashtable.h"

// Response message sent over HTTP
typedef struct Response {
    int status_code;
    char *version;
    HashTable *header;
    char* phrase;
    char *body;
} Response;

// Parses a response object as a string
char* parse_response(Response* response);

// Creates a cookie
char* cookie_generator();

// Deallocates memory from Response struct
void free_response(Response* resp);

// Creates a cookie for a new user
Response* initialise_session(Request* request);
