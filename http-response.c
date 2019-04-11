#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "http-parser.h"
#include "http-response.h"

#define MAX_URL_SIZE 60
#define MAX_HEADER_SIZE 300
#define MAX_VERSION_SIZE 10

Response* redirect(Request* request_message){
    
}

void free_response(Response* resp){
    free_hash_table(resp->header);
    free(resp);
}