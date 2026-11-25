#ifndef GRAPH_H
#define GRAPH_H

#include "config.h"
#include "route.h"

/* Graph tak berarah berbobot (jarak km): simpul = kota, sisi = rute. */
typedef struct {
    char names[MAX_CITY][MAX_CITY_NAME];
    int adj[MAX_CITY][MAX_CITY];   /* 0 = tidak ada sisi */
    int cityCount;
} Graph;

void graph_init(Graph *graph);
int  graph_find_city(const Graph *graph, const char *name);
int  graph_add_city(Graph *graph, const char *name);
void graph_add_edge(Graph *graph, int from, int to, int distance);
void graph_build_from_routes(Graph *graph, const RouteList *list);
void graph_print_connections(const Graph *graph);
void graph_print_cities(const Graph *graph);
void graph_print_bfs(const Graph *graph, int start);
void graph_print_shortest_transit(const Graph *graph, int start, int goal);
void graph_print_dfs(const Graph *graph, int start);

#endif
