#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "response-formatter.h"

static char const ROUND_1='2';
static char const ROUND_2='3';
static char const ROUND_3='4';
static char const ROUND_4='1';

char *substring(char *string, int position, int length)
{
   char *p;
   int c;
   p = calloc(length+1, sizeof(char));
   assert(p);
    // printf("size of length %d\n", length);
   for(c = 0 ; c < length ; c++ )
      *(p+c) = *((string+position-1)+c);      
       
   *(p+c) = '\0';
 
   return p;
}
void insert_substring(char *a, char *b, int position)
{
   char *start, *end;
   int length;
   
   length = strlen(a);
   
   start = substring(a, 1, position);      
   end = substring(a, position+1, length-position);
   sprintf(a, "%s%s%s", start, b, end);
   free(start);
   free(end);
}

void clean_trailing_buffer(char* buff){
    const char needle[10] = "</html>";
    char *result = strstr(buff, needle)+strlen(needle);
    int position = result - buff;
    memcpy(buff, buff, position);
}

void render_text(char* buff, char* text){
    const char new_needle[10] = "\">";
    char* result = strstr(buff, new_needle)+strlen(new_needle);
    int position = result - buff;
    insert_substring(buff, text, position);
}

void change_game_image(char* buff, int round){
    const char needle[10] = "/image-";
    char* result = strstr(buff, needle)+strlen(needle);
    int position = result - buff;
    switch (round){
        case 1:
            buff[position] = ROUND_1;
            break;
        case 2:
            buff[position] = ROUND_2;
            break;
        case 3:
            buff[position] = ROUND_3;
            break;
        case 4:
            buff[position] = ROUND_4;
            break;
        default:
            printf("Invalid Round");
            break;

    }
}