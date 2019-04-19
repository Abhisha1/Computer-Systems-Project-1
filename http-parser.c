#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "http-parser.h"

#define MAX_URL_SIZE 60
#define MAX_HEADER_SIZE 600
#define MAX_VERSION_SIZE 10

Request* parse_request(char* request_message){
    Request *req = calloc(1, sizeof(Request));
	assert(req);
    memset(req, 0, sizeof(Request));
    int n_headers = 10;
    if (strncmp(request_message, "GET ", 4) == 0){
        req->method = GET;
        request_message += 4;
        // printf("Its a get\n");
    }
    else if (strncmp(request_message, "POST ", 5) == 0){
        request_message += 5;
        req->method = POST;
        // printf("Its a post\n");
    }
    req->url = strtok(request_message, " ");
    request_message += strlen(req->url)+1;
    req->version = strtok(request_message, "\r\n");
    request_message+= strlen(req->version)+2;
    req->header = new_hash_table(10);

    char header_field_name[MAX_HEADER_SIZE];
    char header_value[MAX_HEADER_SIZE];
    while(*request_message != '\r' && *request_message != '\n'){
    //    printf("req message first char %d\n", *request_message);
       strcpy(header_field_name,strtok(request_message, " "));
    //    printf("header field name: %s\n", header_field_name);
       request_message += strcspn(request_message, " ")+1;
       strcpy(header_value,strtok(request_message, "\r\n"));
    //    printf("header value: %s\n", header_value);
       request_message += strcspn(request_message, "\r\n")+2;
       hash_table_put(req->header, header_field_name, header_value);
    }
    request_message += strcspn(request_message, "\r\n")+2;
    if(*request_message != '\r'||*request_message != '\n'){
        req->body = request_message;
    }
    else{
        req->body = "";
    }
    return req;
}

void free_request(Request* req){
   free_hash_table(req->header);
    free(req);
}
