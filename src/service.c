#include <stdio.h>
#include <string.h>
#include "service.h"

int service_create_booking(App *app, const char *username, Route *route, int jumlah)
{
    Booking b;

    if (route == NULL || jumlah <= 0 || route->kursi < jumlah) {
        return 0;
    }
    b.id = booking_next_id(&app->bookings);
    strncpy(b.username, username, MAX_USERNAME);
    b.username[MAX_USERNAME] = '\0';
    b.routeId = route->id;
    b.jumlah = jumlah;
    b.status = BOOKING_ACTIVE;

    if (!booking_list_add(&app->bookings, &b)) {
        return 0;
    }
    route->kursi -= jumlah;
    stack_push(&app->undoStack, b.id);
    app_save(app);
    return b.id;
}

int service_cancel_booking(App *app, int bookingId, const char *username)
{
    Booking *b = booking_list_find(&app->bookings, bookingId);
    Route *r;

    if (b == NULL) {
        return CANCEL_NOT_FOUND;
    }
    if (strcmp(b->username, username) != 0) {
        return CANCEL_NOT_OWNER;
    }
    if (b->status == BOOKING_CANCELLED) {
        return CANCEL_ALREADY;
    }
    b->status = BOOKING_CANCELLED;
    r = bst_search(app->routeTree, b->routeId);
    if (r != NULL) {
        r->kursi += b->jumlah;
        service_process_waiting(app, r->id);
    }
    app_save(app);
    return CANCEL_OK;
}

int service_undo_last(App *app, const char *username)
{
    int lastId, res;

    while (stack_pop(&app->undoStack, &lastId)) {
        res = service_cancel_booking(app, lastId, username);
        if (res == CANCEL_OK) {
            return lastId;
        }
    }
    return 0;
}

int service_join_waiting(App *app, const char *username, int routeId, int jumlah)
{
    WaitEntry entry;

    strncpy(entry.username, username, MAX_USERNAME);
    entry.username[MAX_USERNAME] = '\0';
    entry.routeId = routeId;
    entry.jumlah = jumlah;

    if (!enqueue(&app->waiting, &entry)) {
        return 0;
    }
    queue_save_file(&app->waiting, FILE_WAITING);
    return 1;
}

int service_process_waiting(App *app, int routeId)
{
    Queue temp;
    WaitEntry entry;
    Route *r = bst_search(app->routeTree, routeId);
    int processed = 0;

    if (r == NULL) {
        return 0;
    }
    queue_init(&temp);
    while (dequeue(&app->waiting, &entry)) {
        if (entry.routeId == routeId && r->kursi >= entry.jumlah) {
            Booking b;
            b.id = booking_next_id(&app->bookings);
            strncpy(b.username, entry.username, MAX_USERNAME);
            b.username[MAX_USERNAME] = '\0';
            b.routeId = routeId;
            b.jumlah = entry.jumlah;
            b.status = BOOKING_ACTIVE;
            if (booking_list_add(&app->bookings, &b)) {
                r->kursi -= entry.jumlah;
                processed++;
                printf("[Waiting List] Tiket otomatis diterbitkan untuk %s (ID Booking: %d).\n",
                       entry.username, b.id);
            } else {
                enqueue(&temp, &entry);
            }
        } else {
            enqueue(&temp, &entry);
        }
    }
    while (dequeue(&temp, &entry)) {
        enqueue(&app->waiting, &entry);
    }
    queue_free(&temp);
    return processed;
}

void service_remove_route(App *app, int routeId)
{
    int idx = route_list_index_of(&app->routes, routeId);

    if (idx == -1) {
        return;
    }
    app->routeTree = bst_delete(app->routeTree, routeId);
    route_list_remove_at(&app->routes, idx);
    graph_build_from_routes(&app->graph, &app->routes);
    app_save(app);
}
