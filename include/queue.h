#ifndef QUEUE_H
#define QUEUE_H

#include "config.h"

/* Queue (linked list) untuk antrean tunggu tiket (waiting list). */
typedef struct {
    char username[MAX_USERNAME + 1];
    int routeId;
    int jumlah;
} WaitEntry;

typedef struct QueueNode {
    WaitEntry data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

void queue_init(Queue *queue);
int  queue_is_empty(const Queue *queue);
int  enqueue(Queue *queue, const WaitEntry *entry);
int  dequeue(Queue *queue, WaitEntry *entry);
void queue_free(Queue *queue);
int  queue_load_file(Queue *queue, const char *filename);
int  queue_save_file(const Queue *queue, const char *filename);

#endif
