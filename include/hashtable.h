#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "config.h"
#include "user.h"

/* Hash table (separate chaining) untuk pencarian akun berdasarkan username. */
typedef struct UserNode {
    User data;
    struct UserNode *next;
} UserNode;

typedef struct {
    UserNode *bucket[HASH_SIZE];
    int count;
    int collisions;
} UserTable;

unsigned long hash_string(const char *text);
unsigned int  hash_index(const char *key);
void  table_init(UserTable *table);
int   table_insert(UserTable *table, const User *user);
User *table_search(UserTable *table, const char *username);
void  table_free(UserTable *table);
void  table_print_stats(const UserTable *table);

#endif
