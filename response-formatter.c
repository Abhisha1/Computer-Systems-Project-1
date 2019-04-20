/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program deals with all html manipulation, including insertion, cleaning and 
 * dynamic images for each round.
 * */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "response-formatter.h"

//constants which define the image src for each round
static char const ROUND_1='2';
static char const ROUND_2='3';
static char const ROUND_3='4';
static char const ROUND_4='1';

char *substring(char *string, int position, int length){
    /**
     * Returns the substring of from a given position, for a given size
     * */
    char *substring;
    int i;
    substring = calloc(length+1, sizeof(char));
    assert(substring);
    for(i = 0 ; i < length ; i++ )
        // adds characters from given position of string
        *(substring+i) = *((string+position-1)+i);      
        
    *(substring+i) = '\0';

    return substring;
}
void insert_substring(char *dest, char *insert_text, int position){
    /**
     * Inserts a string into another string, at a given position
    */
    char *start, *end;
    int length;

    length = strlen(dest);
    // gets first half of string
    start = substring(dest, 1, position);
    // gets second half a string      
    end = substring(dest, position+1, length-position);
    // formats the string with new string in the middle
    sprintf(dest, "%s%s%s", start, insert_text, end);
    free(start);
    free(end);
}

void clean_trailing_buffer(char* buff){
    /**
     * Cleans any trailing characters after end of html tag
     * */
    const char needle[10] = "</html>";
    // gets position of final html tag
    char *result = strstr(buff, needle)+strlen(needle);
    int position = result - buff;
    // overwrites buff to exclude trailing characters
    memcpy(buff, buff, position);
}

void render_text(char* buff, char* text){
    /**
     * Inserts text into buff after the image
     * */
    const char new_needle[10] = "\">";
    char* result = strstr(buff, new_needle)+strlen(new_needle);
    int position = result - buff;
    insert_substring(buff, text, position);
}

void change_game_image(char* buff, int round){
    /**
     * Changes the image src accordingly to the game round
     * */
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