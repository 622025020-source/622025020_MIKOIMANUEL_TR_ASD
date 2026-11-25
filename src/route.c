#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route.h"
#include "util.h"

#define MINUTES_PER_HOUR 60
#define TIME_TEXT_SIZE   16

void route_list_init(RouteList *list)
{
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    list->items = (Route **)malloc(sizeof(Route *) * (size_t)list->capacity);
    if (list->items == NULL) {
        printf("Memori tidak cukup.\n");
        exit(1);
    }
}

void route_list_free(RouteList *list)
{
    int i;

    if (list->items == NULL) {
        return;
    }
    for (i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* List mengambil alih kepemilikan route. Kapasitas digandakan bila penuh. */
int route_list_add(RouteList *list, Route *route)
{
    Route **bigger;

    if (list->count == list->capacity) {
        bigger = (Route **)realloc(list->items,
                                   sizeof(Route *) * (size_t)list->capacity * 2);
        if (bigger == NULL) {
            return 0;
        }
        list->items = bigger;
        list->capacity *= 2;
    }
    list->items[list->count++] = route;
    return 1;
}

void route_list_remove_at(RouteList *list, int index)
{
    int i;

    if (index < 0 || index >= list->count) {
        return;
    }
    free(list->items[index]);
    for (i = index; i < list->count - 1; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
}

int route_list_index_of(const RouteList *list, int id)
{
    int i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i]->id == id) {
            return i;
        }
    }
    return -1;
}

int route_next_id(const RouteList *list)
{
    int i;
    int max = 0;

    for (i = 0; i < list->count; i++) {
        if (list->items[i]->id > max) {
            max = list->items[i]->id;
        }
    }
    return max + 1;
}

Route *route_create(int id, const char *asal, const char *tujuan,
                    const char *jenis, int harga, int jarak, int jam, int kursi)
{
    Route *route = (Route *)malloc(sizeof(Route));

    if (route == NULL) {
        return NULL;
    }
    route->id = id;
    strncpy(route->asal, asal, MAX_CITY_NAME - 1);
    route->asal[MAX_CITY_NAME - 1] = '\0';
    strncpy(route->tujuan, tujuan, MAX_CITY_NAME - 1);
    route->tujuan[MAX_CITY_NAME - 1] = '\0';
    strncpy(route->jenis, jenis, MAX_TYPE_NAME - 1);
    route->jenis[MAX_TYPE_NAME - 1] = '\0';
    route->harga = harga;
    route->jarak = jarak;
    route->jam = jam;
    route->kursi = kursi;
    return route;
}

/* Format baris: id|asal|tujuan|jenis|harga|jarak|HH:MM|kursi */
int route_load(RouteList *list, const char *filename)
{
    char line[MAX_LINE];
    char asal[MAX_CITY_NAME], tujuan[MAX_CITY_NAME], jenis[MAX_TYPE_NAME];
    int id, harga, jarak, hour, minute, kursi;
    Route *route;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%29[^|]|%29[^|]|%14[^|]|%d|%d|%d:%d|%d",
                   &id, asal, tujuan, jenis, &harga, &jarak,
                   &hour, &minute, &kursi) != 9) {
            continue;
        }
        route = route_create(id, asal, tujuan, jenis, harga, jarak,
                             hour * MINUTES_PER_HOUR + minute, kursi);
        if (route != NULL && !route_list_add(list, route)) {
            free(route);
        }
    }
    fclose(file);
    return 1;
}

#define HM(h, m) ((h) * MINUTES_PER_HOUR + (m))

typedef struct {
    const char *asal;
    const char *tujuan;
    const char *jenis;
    int harga;
    int jarak;
    int jam;
    int kursi;
} DefaultRoute;

static const DefaultRoute DEFAULT_ROUTES[] = {
    /* Jawa Tengah */
    {"Salatiga",   "Semarang",   "Bus",    30000,   50, HM(6, 0),   40},
    {"Semarang",   "Salatiga",   "Bus",    30000,   50, HM(8, 30),  40},
    {"Salatiga",   "Surakarta",  "Bus",    35000,   55, HM(7, 0),   40},
    {"Surakarta",  "Salatiga",   "Travel", 50000,   55, HM(9, 15),  12},
    {"Salatiga",   "Yogyakarta", "Travel", 85000,  100, HM(8, 0),   12},
    {"Semarang",   "Surakarta",  "Kereta", 90000,  100, HM(7, 30),  80},
    {"Semarang",   "Yogyakarta", "Bus",    75000,  120, HM(10, 0),  40},
    {"Surakarta",  "Yogyakarta", "Kereta", 20000,   65, HM(6, 45),  90},
    {"Yogyakarta", "Surakarta",  "Kereta", 20000,   65, HM(16, 0),  90},
    {"Purwokerto", "Yogyakarta", "Kereta", 100000, 190, HM(9, 0),   70},
    /* Jakarta - Jawa Barat & Jawa Tengah */
    {"Jakarta",    "Bandung",    "Kereta", 150000, 150, HM(6, 0),   80},
    {"Bandung",    "Jakarta",    "Kereta", 150000, 150, HM(17, 30), 80},
    {"Jakarta",    "Cirebon",    "Kereta", 200000, 260, HM(8, 0),   60},
    {"Jakarta",    "Semarang",   "Kereta", 350000, 450, HM(20, 0),  70},
    {"Jakarta",    "Salatiga",   "Travel", 300000, 490, HM(19, 0),  12},
    {"Jakarta",    "Surakarta",  "Kereta", 400000, 540, HM(21, 0),  70},
    {"Jakarta",    "Yogyakarta", "Kereta", 380000, 520, HM(19, 30), 70},
    {"Bandung",    "Cirebon",    "Travel", 90000,  130, HM(7, 0),   12},
    {"Bandung",    "Yogyakarta", "Kereta", 320000, 430, HM(18, 0),  60},
    {"Cirebon",    "Semarang",   "Bus",    110000, 230, HM(9, 30),  40},
    {"Cirebon",    "Purwokerto", "Kereta", 80000,  150, HM(11, 0),  60},
    /* Jakarta - Surabaya & Jawa Timur */
    {"Jakarta",    "Surabaya",   "Kereta", 500000, 780, HM(16, 0),  90},
    {"Surabaya",   "Jakarta",    "Kereta", 500000, 780, HM(15, 0),  90},
    {"Semarang",   "Surabaya",   "Kereta", 250000, 350, HM(13, 0),  70},
    {"Salatiga",   "Surabaya",   "Bus",    160000, 310, HM(11, 30), 40},
    {"Surakarta",  "Surabaya",   "Kereta", 180000, 260, HM(14, 0),  80},
    {"Yogyakarta", "Surabaya",   "Kereta", 210000, 320, HM(7, 0),   80},
    {"Surakarta",  "Malang",     "Bus",    170000, 280, HM(8, 0),   40},
    {"Surabaya",   "Malang",     "Travel", 70000,   90, HM(6, 30),  12},
    {"Malang",     "Surabaya",   "Travel", 70000,   90, HM(15, 30), 12},
    /* Bali */
    {"Surabaya",   "Denpasar",   "Bus",    300000, 420, HM(19, 0),  40},
    {"Malang",     "Denpasar",   "Bus",    330000, 480, HM(18, 0),  40}
};

#define DEFAULT_ROUTE_COUNT ((int)(sizeof(DEFAULT_ROUTES) / sizeof(DEFAULT_ROUTES[0])))

int route_load_defaults(RouteList *list)
{
    int i;
    int added = 0;
    Route *route;
    const DefaultRoute *d;

    for (i = 0; i < DEFAULT_ROUTE_COUNT; i++) {
        d = &DEFAULT_ROUTES[i];
        route = route_create(route_next_id(list), d->asal, d->tujuan, d->jenis,
                             d->harga, d->jarak, d->jam, d->kursi);
        if (route == NULL) {
            continue;
        }
        if (route_list_add(list, route)) {
            added++;
        } else {
            free(route);
        }
    }
    return added;
}

int route_save(const RouteList *list, const char *filename)
{
    int i;
    const Route *r;
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Gagal menyimpan %s.\n", filename);
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        r = list->items[i];
        fprintf(file, "%d|%s|%s|%s|%d|%d|%02d:%02d|%d\n", r->id, r->asal,
                r->tujuan, r->jenis, r->harga, r->jarak,
                r->jam / MINUTES_PER_HOUR, r->jam % MINUTES_PER_HOUR, r->kursi);
    }
    fclose(file);
    return 1;
}

void route_time_to_string(int minutes, char *out, size_t size)
{
    snprintf(out, size, "%02d:%02d", minutes / MINUTES_PER_HOUR,
             minutes % MINUTES_PER_HOUR);
}

void route_print_header(void)
{
    print_line();
    printf("%-5s %-14s %-14s %-8s %-11s %-7s %-6s %s\n", "ID", "Asal", "Tujuan",
           "Jenis", "Harga (Rp)", "Jarak", "Jam", "Kursi");
    print_line();
}

void route_print_row(const Route *route)
{
    char time[TIME_TEXT_SIZE];

    route_time_to_string(route->jam, time, sizeof(time));
    printf("%-5d %-14s %-14s %-8s %-11d %4d km %-6s %d\n", route->id,
           route->asal, route->tujuan, route->jenis, route->harga,
           route->jarak, time, route->kursi);
}

void route_print_array(Route **items, int count)
{
    int i;

    route_print_header();
    for (i = 0; i < count; i++) {
        route_print_row(items[i]);
    }
    print_line();
    printf("Total: %d rute\n", count);
}
