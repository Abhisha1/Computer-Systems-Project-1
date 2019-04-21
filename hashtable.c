/* This hash table has been adapted by Abhisha Nirmalathas.
	The original hash table it was adapted from was made
 by Matt Farrugia <matt.farrugia@unimelb.edu.au, for Design of Algorithms.
 This hash table uses separate chaining with an array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hashtable.h"

typedef struct bucket Bucket;
struct bucket {
	char **keys; //array of words
	char **values; //array of weighted frequencies, lower value is higher frequency
	int capacity; //space available in bucket
	int n_elems; //number of elements in bucket
};

struct table {
	int size; //number of buckets
	Bucket *buckets; //list of buckets
};

/************************************************************************/
// moves recently accessed key & value to the front of bucket
void move_to_front(Bucket *bucket, int hash_value);

 unsigned int seed = 73802;
 unsigned int xor_hash(const char *key, unsigned int size) {
 	unsigned int h = seed;

 	int i;
 	for (i = 0; key[i] != '\0'; i++) {
 		h = h ^ ((h << 5) + key[i] + (h >> 2));
 	}

 	return h % size;
 }
/************************************************************************/
Bucket *add_to_bucket(Bucket *bucket, char *key, char *value) {
	/*Adds new key and value to existing bucket*/
	bucket->n_elems = bucket->n_elems + 1;
	// allocates more memory if not enough space
	if (bucket->capacity <= bucket->n_elems){
		bucket->capacity = 2*(bucket->capacity + 1);
		bucket->keys = realloc(bucket->keys,sizeof(char*) * (bucket->capacity));
		bucket->values = realloc(bucket->values,sizeof(char*) * (bucket->capacity));
	}
	assert(bucket->keys);
	assert(bucket->values);
	//adds new key and avlue to end of bucket
	bucket->values[bucket->n_elems-1] = value;
	bucket->keys[bucket->n_elems-1] = key;
	// printf("key is %s\n\n", bucket->keys[bucket->n_elems-1]);
	// printf("value is %s\n\n", bucket->values[bucket->n_elems-1]);
	return bucket;
}
/************************************************************************/
// Warning: does not free bucket->next
void free_bucket(Bucket bucket) {
	/*Deallocates memory in bucket*/
	free(bucket.keys);
	free(bucket.values);
}


/************************************************************************/
HashTable *new_hash_table(int size) {
	/*Builds a new hash table of input size*/
	HashTable *table = malloc(sizeof *table);
	assert(table);
	table->size = size;
	table->buckets = malloc(size * (sizeof *table->buckets));
	assert(table->buckets);
	int i;
	for (i = 0; i < size; i++) {
		table->buckets[i].keys = malloc(sizeof(char*));
		table->buckets[i].values = malloc(sizeof(char*));
		table->buckets[i].capacity = 0;
		table->buckets[i].n_elems = 0;
	}
	return table;
}
/************************************************************************/
void free_hash_table(HashTable *table) {
	/*Deallocates memory from hash table*/
	assert(table != NULL);
	int i;
	for (i = 0; i < table->size; i++) {
		free_bucket(table->buckets[i]);
	}
	free(table->buckets);
	free(table);
}

/************************************************************************/
bool equal(char *a, char *b) {
	/*Returns if a string is equal to another string*/
	return strcmp(a, b) == 0;
}

/************************************************************************/
void hash_table_put(HashTable *table, char *key, char *value) {
	/*Inserts new key into hash table with weighted value*/
	assert(table != NULL);
	assert(key != NULL);
	int hash_value = xor_hash(key, table->size);
	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	printf("the number of elements in bucket is %d\n", bucket->n_elems);
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
			bucket->values[i] = value;
			return;
		}
	}
	// adds key and value as not in hash table
	bucket = add_to_bucket(bucket, key, value);
	table->buckets[hash_value] = *bucket;
	printf("the number of elements in bucket is %d\n", bucket->n_elems);
}
/************************************************************************/
void move_to_front(Bucket *bucket, int index){
	/*moves recently accessed key & value to the front of bucket*/
	char* temp_key;
	char* temp_value;
	if (bucket->n_elems <= 1 && index >= bucket->n_elems){
		return;
	}
	// swaps position of first element with recently accessed
	temp_key = bucket->keys[index];
	temp_value = bucket->values[index];
	bucket->keys[index] = bucket->keys[0];
	bucket->values[index] = bucket->values[0];
	bucket->values[0] = temp_value;
	bucket->keys[0] = temp_key;
}
/************************************************************************/
char* hash_table_get(HashTable *table, char *key) {
	/*Checks if key in hash table and returns value if present*/
	char* freq = calloc(60,sizeof(char));
	assert(freq);
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = xor_hash(key, table->size);

	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
			memcpy(freq, bucket->values[i], strlen(bucket->values[i]));
			move_to_front(bucket, i);
			break;
		}
	}
	//returns value of key if found, else return -1
	return freq;
}
/************************************************************************/
bool hash_table_has(HashTable *table, char *key) {
	/*Checks if the key exists in hash table*/
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = xor_hash(key, table->size);

	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
			return true;
		}
	}
	return false;
}
char* print_hash_map(HashTable *table){
	char* headers = calloc(800,sizeof(char));
	assert(headers);
	for (int i = 0; i < table->size; i++) {
		if (table->buckets[i].n_elems > 0){
			for (int j=0; j< table->buckets[i].n_elems; j++){
				strcat(headers, table->buckets[i].keys[j]);
				strcat(headers, table->buckets[i].values[j]);
				strcat(headers, "\r\n");
			}
		}
	}
	return headers;
}

void free_cookie(HashTable *headers) {
	/*Checks if key in hash table and returns value if present*/

	assert(headers != NULL);

	int hash_value = xor_hash("Set-cookie: ", headers->size);

	// look for existing key
	Bucket *bucket = &headers->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], "Set-cookie: ")){
			free(bucket->values[i]);
		}
	}
}

void free_hash_map(HashTable *table){
	for (int i = 0; i < table->size; i++) {
		if (table->buckets[i].n_elems > 0){
			for (int j=0; j< table->buckets[i].n_elems; j++){
				free(table->buckets[i].keys[j]);
				free(table->buckets[i].values[j]);
			}
		}
	}
}
