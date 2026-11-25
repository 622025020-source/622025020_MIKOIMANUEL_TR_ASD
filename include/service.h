#ifndef SERVICE_H
#define SERVICE_H

#include "app.h"

#define CANCEL_OK         1
#define CANCEL_NOT_FOUND  0
#define CANCEL_NOT_OWNER  (-1)
#define CANCEL_ALREADY    (-2)

int  service_create_booking(App *app, const char *username, Route *route, int jumlah);
int  service_cancel_booking(App *app, int bookingId, const char *username);
int  service_undo_last(App *app, const char *username);
int  service_join_waiting(App *app, const char *username, int routeId, int jumlah);
int  service_process_waiting(App *app, int routeId);
void service_remove_route(App *app, int routeId);

#endif
