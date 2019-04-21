/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program deals with the parsing of http requests
 * */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "http-parser.h"
#include "hashtable.h"

// user-determined constants for requests
#define MAX_URL_SIZE 60
#define MAX_HEADER_SIZE 200
#define MAX_VERSION_SIZE 10
#define MAX_HEADERS 20

Request* parse_request(char* request_message){
    /**
     * Reads a requests and creates a request object containing the parsed information
     * */
    Request *req = calloc(1, sizeof *req);
	assert(req);

    // parses the method of the request
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

    //parses the url 
    req->url = strtok(request_message, " ");
    request_message += strlen(req->url)+1;

    //parses the version
    req->version = strtok(request_message, "\r\n");
    request_message+= strlen(req->version)+2;

    //parses the headers
    req->header = new_hash_table(MAX_HEADERS);

    // char *header_field_name = calloc(MAX_HEADER_SIZE, sizeof(char));
    // char *header_value = calloc(MAX_HEADER_SIZE, sizeof(char));
    while(*request_message != '\r' && *request_message != '\n'){
        request_message = insert_headers(request_message, req);
    //    printf("req message first char %d\n", *request_message);
    //     memset(header_field_name,0,strlen(header_field_name));
    //     memset(header_value,0,strlen(header_value));
    //     size_t field_len = strcspn(request_message, " ");
    //     memcpy(header_field_name, request_message,field_len);
    //     request_message += field_len+1;
    //     size_t value_len = strcspn(request_message, "\r\n");
    //     memcpy(header_value,request_message, value_len);
    //    printf("header key: %s\n", header_field_name);
    //    request_message += value_len+2;
    //    hash_table_put(req->header, header_field_name, header_value);
    }
    request_message += strcspn(request_message, "\r\n")+2;
    printf("request header %s\n", print_hash_map(req->header));
    // parses the body
    if(*request_message != '\r'||*request_message != '\n'){
        req->body = request_message;
    }
    else{
        req->body = "";
    }
    return req;
}

char* insert_headers(char* request_message, Request * req){
    char *header_field_name = calloc(MAX_HEADER_SIZE, sizeof(char));
    char *header_value = calloc(MAX_HEADER_SIZE, sizeof(char));
    memset(header_field_name,0,strlen(header_field_name));
        memset(header_value,0,strlen(header_value));
        size_t field_len = strcspn(request_message, " ");
        memcpy(header_field_name, request_message,field_len);
        request_message += field_len+1;
        size_t value_len = strcspn(request_message, "\r\n");
        memcpy(header_value,request_message, value_len);
       printf("header key: %s\n", header_field_name);
       request_message += value_len+2;
       hash_table_put(req->header, header_field_name, header_value);
       return request_message;
}

void free_request(Request* req){
    /**
     * Deallocates memory
     * */
    free_hash_map(req->header);
    free_hash_table(req->header);
    // free(req);
}
