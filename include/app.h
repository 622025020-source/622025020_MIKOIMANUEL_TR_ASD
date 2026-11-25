#ifndef APP_H
#define APP_H

#include "hashtable.h"
#include "route.h"
#include "bst.h"
#include "graph.h"
#include "booking.h"
#include "queue.h"
#include "stack.h"

/* Seluruh state program dikumpulkan di satu struct dan dioper lewat pointer. */
typedef struct {
    UserTable users;       /* hash table akun            */
    RouteList routes;      /* array dinamis rute         */
    BstNode *routeTree;    /* BST indeks ID rute         */
    Graph graph;           /* graph jaringan kota        */
    BookingList bookings;  /* array dinamis pemesanan    */
    Queue waiting;         /* queue antrean tunggu       */
    Stack undoStack;       /* stack riwayat sesi / undo  */
    User *current;         /* akun yang sedang login     */
} App;

void app_init(App *app);
void app_load(App *app);
int  app_save(const App *app);
void app_rebuild_indexes(App *app);
void app_free(App *app);

#endif
