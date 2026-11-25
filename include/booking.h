#ifndef BOOKING_H
#define BOOKING_H

#include "config.h"

#define BOOKING_ACTIVE     0
#define BOOKING_CANCELLED  1

typedef struct {
    int id;
    char username[MAX_USERNAME + 1];
    int routeId;
    int jumlah;
    int status;
} Booking;

/* Array dinamis (malloc/realloc) berisi data pemesanan. */
typedef struct {
    Booking *items;
    int count;
    int capacity;
} BookingList;

void     booking_list_init(BookingList *list);
void     booking_list_free(BookingList *list);
int      booking_list_add(BookingList *list, const Booking *booking);
Booking *booking_list_find(BookingList *list, int id);
int      booking_next_id(const BookingList *list);
int      booking_load(BookingList *list, const char *filename);
int      booking_save(const BookingList *list, const char *filename);

#endif
