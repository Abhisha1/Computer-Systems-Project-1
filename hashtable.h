/* This hash table has been adapted by Abhisha Nirmalathas.
	The original hash table it was adapted from was made
 by Matt Farrugia <matt.farrugia@unimelb.edu.au, for Design of Algorithms.
 This hash table uses separate chaining with an array.
*/
#include <stdbool.h>

typedef struct table HashTable;


//Creates a new hash table of input size
HashTable *new_hash_table(int size);

//Deallocates memory from hash table
void free_hash_table(HashTable *table);

//Inserts new key into hash table with weighted value
void hash_table_put(HashTable *table, char *key, char *value);

//checks if key in hash table and returns value if present
char* hash_table_get(HashTable *table, char *key);

//Checks if the key exists in hash table
bool hash_table_has(HashTable *table, char *key);
