#include <stdbool.h>
#include "hashtable.h"
typedef enum
{
    GET,
    POST,
    UNKNOWN
} METHOD;

typedef struct Request {
    METHOD method;
    char *url;
    char *version;
    HashTable *header;
    char *body;
} Request;


//Creates a new hash table of input size
Request* parse_request(char* request_message);

void free_request(Request* req);