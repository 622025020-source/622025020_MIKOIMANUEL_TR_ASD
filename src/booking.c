#include <stdio.h>
#include <stdlib.h>
#include "booking.h"

void booking_list_init(BookingList *list)
{
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    list->items = (Booking *)malloc(sizeof(Booking) * (size_t)list->capacity);
    if (list->items == NULL) {
        printf("Memori tidak cukup.\n");
        exit(1);
    }
}

void booking_list_free(BookingList *list)
{
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int booking_list_add(BookingList *list, const Booking *booking)
{
    Booking *bigger;

    if (list->count == list->capacity) {
        bigger = (Booking *)realloc(list->items,
                                    sizeof(Booking) * (size_t)list->capacity * 2);
        if (bigger == NULL) {
            return 0;
        }
        list->items = bigger;
        list->capacity *= 2;
    }
    list->items[list->count++] = *booking;
    return 1;
}

Booking *booking_list_find(BookingList *list, int id)
{
    int i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i].id == id) {
            return &list->items[i];
        }
    }
    return NULL;
}

int booking_next_id(const BookingList *list)
{
    int i, max = 0;

    for (i = 0; i < list->count; i++) {
        if (list->items[i].id > max) {
            max = list->items[i].id;
        }
    }
    return max + 1;
}

int booking_load(BookingList *list, const char *filename)
{
    char line[MAX_LINE];
    Booking b;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%20[^|]|%d|%d|%d",
                   &b.id, b.username, &b.routeId, &b.jumlah, &b.status) == 5) {
            booking_list_add(list, &b);
        }
    }
    fclose(file);
    return 1;
}

int booking_save(const BookingList *list, const char *filename)
{
    int i;
    const Booking *b;
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        b = &list->items[i];
        fprintf(file, "%d|%s|%d|%d|%d\n",
                b->id, b->username, b->routeId, b->jumlah, b->status);
    }
    fclose(file);
    return 1;
}
