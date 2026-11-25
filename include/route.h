#ifndef ROUTE_H
#define ROUTE_H

#include <stddef.h>
#include "config.h"

typedef struct {
    int id;
    char asal[MAX_CITY_NAME];
    char tujuan[MAX_CITY_NAME];
    char jenis[MAX_TYPE_NAME];
    int harga;   /* rupiah per kursi */
    int jarak;   /* kilometer */
    int jam;     /* jam berangkat, menit sejak 00:00 */
    int kursi;   /* sisa kursi */
} Route;

/* Array dinamis berisi pointer Route (setiap Route di-malloc sendiri). */
typedef struct {
    Route **items;
    int count;
    int capacity;
} RouteList;

void   route_list_init(RouteList *list);
void   route_list_free(RouteList *list);
int    route_list_add(RouteList *list, Route *route);
void   route_list_remove_at(RouteList *list, int index);
int    route_list_index_of(const RouteList *list, int id);
int    route_next_id(const RouteList *list);
Route *route_create(int id, const char *asal, const char *tujuan,
                    const char *jenis, int harga, int jarak, int jam, int kursi);
int    route_load(RouteList *list, const char *filename);
int    route_load_defaults(RouteList *list);
int    route_save(const RouteList *list, const char *filename);
void   route_time_to_string(int minutes, char *out, size_t size);
void   route_print_header(void);
void   route_print_row(const Route *route);
void   route_print_array(Route **items, int count);

#endif
