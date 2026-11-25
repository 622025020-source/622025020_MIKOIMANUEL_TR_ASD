#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/* Fungsi hash djb2, dipotong 32 bit agar sama di semua platform. */
unsigned long hash_string(const char *text)
{
    unsigned long hash = HASH_SEED;
    int c;

    while ((c = (unsigned char)*text++) != 0) {
        hash = ((hash << 5) + hash) + (unsigned long)c;
        hash &= HASH_MASK;
    }
    return hash;
}

unsigned int hash_index(const char *key)
{
    return (unsigned int)(hash_string(key) % HASH_SIZE);
}

void table_init(UserTable *table)
{
    int i;

    for (i = 0; i < HASH_SIZE; i++) {
        table->bucket[i] = NULL;
    }
    table->count = 0;
    table->collisions = 0;
}

/* Mengembalikan 1 jika berhasil, 0 jika username ganda / memori habis. */
int table_insert(UserTable *table, const User *user)
{
    unsigned int index;
    UserNode *node;

    if (table_search(table, user->username) != NULL) {
        return 0;
    }
    node = (UserNode *)malloc(sizeof(UserNode));
    if (node == NULL) {
        return 0;
    }
    index = hash_index(user->username);
    if (table->bucket[index] != NULL) {
        table->collisions++; /* collision ditangani dengan chaining */
    }
    node->data = *user;
    node->next = table->bucket[index];
    table->bucket[index] = node;
    table->count++;
    return 1;
}

User *table_search(UserTable *table, const char *username)
{
    UserNode *node = table->bucket[hash_index(username)];

    while (node != NULL) {
        if (strcmp(node->data.username, username) == 0) {
            return &node->data;
        }
        node = node->next;
    }
    return NULL;
}

void table_free(UserTable *table)
{
    int i;
    UserNode *node, *next;

    for (i = 0; i < HASH_SIZE; i++) {
        node = table->bucket[i];
        while (node != NULL) {
            next = node->next;
            free(node);
            node = next;
        }
        table->bucket[i] = NULL;
    }
    table->count = 0;
    table->collisions = 0;
}

void table_print_stats(const UserTable *table)
{
    int i;
    UserNode *node;

    printf("Ukuran tabel: %d | Jumlah akun: %d | Jumlah collision: %d\n",
           HASH_SIZE, table->count, table->collisions);
    for (i = 0; i < HASH_SIZE; i++) {
        if (table->bucket[i] == NULL) {
            continue;
        }
        printf("  Bucket %2d:", i);
        for (node = table->bucket[i]; node != NULL; node = node->next) {
            printf(" [%s]", node->data.username);
        }
        printf("\n");
    }
}
