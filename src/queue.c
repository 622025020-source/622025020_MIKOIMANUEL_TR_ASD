#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void queue_init(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int queue_is_empty(const Queue *queue)
{
    return queue->front == NULL;
}

int enqueue(Queue *queue, const WaitEntry *entry)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));

    if (node == NULL) {
        return 0;
    }
    node->data = *entry;
    node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return 1;
}

int dequeue(Queue *queue, WaitEntry *entry)
{
    QueueNode *temp;

    if (queue_is_empty(queue)) {
        return 0;
    }
    temp = queue->front;
    if (entry != NULL) {
        *entry = temp->data;
    }
    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return 1;
}

void queue_free(Queue *queue)
{
    WaitEntry dummy;

    while (dequeue(queue, &dummy)) {
    }
}

int queue_load_file(Queue *queue, const char *filename)
{
    char line[MAX_LINE];
    WaitEntry entry;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%20[^|]|%d|%d", entry.username,
                   &entry.routeId, &entry.jumlah) == 3) {
            enqueue(queue, &entry);
        }
    }
    fclose(file);
    return 1;
}

int queue_save_file(const Queue *queue, const char *filename)
{
    QueueNode *curr;
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return 0;
    }
    for (curr = queue->front; curr != NULL; curr = curr->next) {
        fprintf(file, "%s|%d|%d\n", curr->data.username,
                curr->data.routeId, curr->data.jumlah);
    }
    fclose(file);
    return 1;
}
