#include <stdlib.h>
#include "app.h"
#include "menu.h"

static App g_app;

static void cleanup_on_exit(void)
{
    app_free(&g_app);
}

int main(void)
{
    app_init(&g_app);
    atexit(cleanup_on_exit);
    app_load(&g_app);
    menu_run(&g_app);
    return 0;
}
