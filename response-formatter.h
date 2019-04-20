/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program deals with all html manipulation, including insertion, cleaning and 
 * dynamic images for each round.
 * */
#include <stdbool.h>

// Returns the substring of from a given position, for a given size
char *substring(char *string, int position, int length);

// Inserts a string into another string, at a given position
void insert_substring(char *a, char *b, int position);

// Cleans any trailing characters after end of html tag
void clean_trailing_buffer(char* buff);

// Inserts text into buff after the image
void render_text(char* buff, char* text);

// Changes image of html accordingly to round
void change_game_image(char* buff, int round);