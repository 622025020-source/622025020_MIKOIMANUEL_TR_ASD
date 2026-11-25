#include "auth.h"
#include <stddef.h>
#include "app.h"

void app_init(App *app)
{
    table_init(&app->users);
    route_list_init(&app->routes);
    app->routeTree = NULL;
    graph_init(&app->graph);
    booking_list_init(&app->bookings);
    queue_init(&app->waiting);
    stack_init(&app->undoStack);
    app->current = NULL;
}

void app_rebuild_indexes(App *app)
{
    int i;

    bst_free(app->routeTree);
    app->routeTree = NULL;
    for (i = 0; i < app->routes.count; i++) {
        app->routeTree = bst_insert(app->routeTree, app->routes.items[i]);
    }
    graph_build_from_routes(&app->graph, &app->routes);
}

void app_load(App *app)
{
    auth_load(&app->users);
    if (!route_load(&app->routes, FILE_ROUTES) || app->routes.count == 0) {
        route_load_defaults(&app->routes);
        route_save(&app->routes, FILE_ROUTES);
    }
    booking_load(&app->bookings, FILE_BOOKINGS);
    queue_load_file(&app->waiting, FILE_WAITING);
    app_rebuild_indexes(app);
}

int app_save(const App *app)
{
    int ok = 1;

    ok &= auth_save(&app->users);
    ok &= route_save(&app->routes, FILE_ROUTES);
    ok &= booking_save(&app->bookings, FILE_BOOKINGS);
    ok &= queue_save_file(&app->waiting, FILE_WAITING);
    return ok;
}

void app_free(App *app)
{
    bst_free(app->routeTree);
    app->routeTree = NULL;
    route_list_free(&app->routes);
    booking_list_free(&app->bookings);
    queue_free(&app->waiting);
    stack_free(&app->undoStack);
    table_free(&app->users);
    app->current = NULL;
}
