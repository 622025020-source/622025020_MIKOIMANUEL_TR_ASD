#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "util.h"

void graph_init(Graph *graph)
{
    int i, j;

    graph->cityCount = 0;
    for (i = 0; i < MAX_CITY; i++) {
        graph->names[i][0] = '\0';
        for (j = 0; j < MAX_CITY; j++) {
            graph->adj[i][j] = 0;
        }
    }
}

int graph_find_city(const Graph *graph, const char *name)
{
    int i;

    for (i = 0; i < graph->cityCount; i++) {
        if (str_equal_ci(graph->names[i], name)) {
            return i;
        }
    }
    return -1;
}

int graph_add_city(Graph *graph, const char *name)
{
    int idx = graph_find_city(graph, name);

    if (idx != -1) {
        return idx;
    }
    if (graph->cityCount >= MAX_CITY) {
        return -1;
    }
    idx = graph->cityCount++;
    strncpy(graph->names[idx], name, MAX_CITY_NAME - 1);
    graph->names[idx][MAX_CITY_NAME - 1] = '\0';
    return idx;
}

void graph_add_edge(Graph *graph, int from, int to, int distance)
{
    if (from < 0 || to < 0 || from >= graph->cityCount || to >= graph->cityCount || from == to) {
        return;
    }
    if (graph->adj[from][to] == 0 || distance < graph->adj[from][to]) {
        graph->adj[from][to] = distance;
        graph->adj[to][from] = distance;
    }
}

void graph_build_from_routes(Graph *graph, const RouteList *list)
{
    int i, u, v;
    const Route *r;

    graph_init(graph);
    for (i = 0; i < list->count; i++) {
        r = list->items[i];
        u = graph_add_city(graph, r->asal);
        v = graph_add_city(graph, r->tujuan);
        graph_add_edge(graph, u, v, r->jarak);
    }
}

void graph_print_cities(const Graph *graph)
{
    int i;

    printf("Daftar Kota (%d):\n", graph->cityCount);
    for (i = 0; i < graph->cityCount; i++) {
        printf("  [%2d] %s\n", i, graph->names[i]);
    }
}

void graph_print_connections(const Graph *graph)
{
    int i, j, hasEdge;

    print_title("JARINGAN KONEKSI ANTAR KOTA (GRAPH)");
    for (i = 0; i < graph->cityCount; i++) {
        printf("%-14s -> ", graph->names[i]);
        hasEdge = 0;
        for (j = 0; j < graph->cityCount; j++) {
            if (graph->adj[i][j] > 0) {
                if (hasEdge) {
                    printf(", ");
                }
                printf("%s (%d km)", graph->names[j], graph->adj[i][j]);
                hasEdge = 1;
            }
        }
        if (!hasEdge) {
            printf("(tidak ada koneksi)");
        }
        printf("\n");
    }
}

void graph_print_bfs(const Graph *graph, int start)
{
    int visited[MAX_CITY] = {0};
    int q[MAX_CITY];
    int front = 0, rear = 0, u, v;

    if (start < 0 || start >= graph->cityCount) {
        return;
    }
    visited[start] = 1;
    q[rear++] = start;

    printf("Penelusuran BFS dari %s:\n  ", graph->names[start]);
    while (front < rear) {
        u = q[front++];
        printf("%s", graph->names[u]);
        for (v = 0; v < graph->cityCount; v++) {
            if (graph->adj[u][v] > 0 && !visited[v]) {
                visited[v] = 1;
                q[rear++] = v;
            }
        }
        if (front < rear) {
            printf(" -> ");
        }
    }
    printf("\n");
}

static void dfs_helper(const Graph *graph, int u, int visited[], int *first)
{
    int v;

    visited[u] = 1;
    if (!(*first)) {
        printf(" -> ");
    }
    printf("%s", graph->names[u]);
    *first = 0;

    for (v = 0; v < graph->cityCount; v++) {
        if (graph->adj[u][v] > 0 && !visited[v]) {
            dfs_helper(graph, v, visited, first);
        }
    }
}

void graph_print_dfs(const Graph *graph, int start)
{
    int visited[MAX_CITY] = {0};
    int first = 1;

    if (start < 0 || start >= graph->cityCount) {
        return;
    }
    printf("Penelusuran DFS dari %s:\n  ", graph->names[start]);
    dfs_helper(graph, start, visited, &first);
    printf("\n");
}

void graph_print_shortest_transit(const Graph *graph, int start, int goal)
{
    int visited[MAX_CITY] = {0};
    int parent[MAX_CITY];
    int q[MAX_CITY];
    int path[MAX_CITY];
    int front = 0, rear = 0, u, v, curr, len = 0, i, totalDist = 0;

    if (start < 0 || goal < 0 || start >= graph->cityCount || goal >= graph->cityCount) {
        return;
    }
    for (i = 0; i < MAX_CITY; i++) {
        parent[i] = -1;
    }
    visited[start] = 1;
    q[rear++] = start;

    while (front < rear) {
        u = q[front++];
        if (u == goal) {
            break;
        }
        for (v = 0; v < graph->cityCount; v++) {
            if (graph->adj[u][v] > 0 && !visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                q[rear++] = v;
            }
        }
    }

    if (!visited[goal]) {
        printf("Tidak ada jalur transit dari %s ke %s.\n",
               graph->names[start], graph->names[goal]);
        return;
    }

    curr = goal;
    while (curr != -1) {
        path[len++] = curr;
        curr = parent[curr];
    }

    printf("Rute transit minimum (%d transit) dari %s ke %s:\n  ",
           (len > 1 ? len - 2 : 0), graph->names[start], graph->names[goal]);
    for (i = len - 1; i >= 0; i--) {
        printf("%s", graph->names[path[i]]);
        if (i > 0) {
            totalDist += graph->adj[path[i]][path[i - 1]];
            printf(" -> ");
        }
    }
    printf(" (Total jarak: %d km)\n", totalDist);
}
