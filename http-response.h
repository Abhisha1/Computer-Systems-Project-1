#include <stdbool.h>
#include "hashtable.h"

typedef struct Response {
    int status_code;
    char *version;
    HashTable *header;
    char* phrase;
    char *body;
} Response;


//Creates a new hash table of input size
Response *redirect(Request *req);