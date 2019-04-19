#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "http-parser.h"
#include "http-response.h"

#define MAX_URL_SIZE 60
#define MAX_HEADER_SIZE 300
#define MAX_VERSION_SIZE 10

char* cookie_generator(){
    char* cookie_value;
    cookie_value = (char*) malloc(sizeof(char)*60);
    snprintf(cookie_value, 60, "sessionToken=%d; Expires=Wed, 01 Apr 2019 10:10:10 GMT", rand());
    return cookie_value;
}


Response* initialise_session(Request* request){
    Response *resp = calloc(1, sizeof *resp);
	assert(resp);
    resp->status_code=200;
    resp->version = request->version;
    resp->phrase = "OK";
    resp->header = new_hash_table(2);
    char *cookie = cookie_generator();
    hash_table_put(resp->header, "Set-cookie: ", cookie);
    resp->body="";
    return resp;
}

char* parse_response(Response* response){
    char* response_string;
    char int_buff[4];
    response_string = calloc(100,sizeof(char));
    strcat(response_string, response->version);
    strcat(response_string," ");
    sprintf(int_buff, "%d", response->status_code);
    strcat(response_string, int_buff);
    strcat(response_string, " ");
    strcat(response_string, response->phrase);
    strcat(response_string, "\r\n");
    char* headers = print_hash_map(response->header);
    strncat(response_string, headers, strlen(headers));
    strcat(response_string, "\r\n");
    strcat(response_string, response->body);
    // free(headers);
    return response_string;
}


void free_response(Response* resp){
    free_cookie(resp->header);
    free_hash_table(resp->header);
    free(resp);
}
