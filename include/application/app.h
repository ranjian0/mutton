#ifndef APP_H
#define APP_H

#include <time.h>
#include <sys/time.h>

#include "event.h"
#include "utils/Defs.h"

typedef struct app_t 
{
    void (*init)();
    void (*update)();
    void (*shutdown)();
    const char *window_title;
    int window_width;
    int window_height;
    void *user_data;

    event_t event;
} app_t;

// Global App object
extern app_t app;

/*
 * Entry point function for all applications. 
 * Must be defined and must return an app object
 * 
 * EXAMPLE App.c
 * 
 * app_t app_main() {
 *      return (app_t){
 *          .init = app_init,
 *          .update = app_update,
 *          .shutdown = app_shutdown,
 * 
 *          // Window options not necessary for mobile apps
 *          .window_title = "Example Application",
 *          .window_width = 800,
 *          .window_height = 600,
 * 
 *          // Event callbacks can also be defined
 *          .event.key = on_key,
 *          .event.mouse = on_mouse,
 *          .event.touch = on_touch // Mobile Only
 *          ...
 *      }
 * }
 */
extern app_t app_main();

static void set_app(app_t _app) { app = _app; }
static app_t get_app() { return app; }
static int app_get_width() { return app.window_width; }
static int app_get_height() { return app.window_height; }
static void* app_get_userdata() { return app.user_data; }

static double app_get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}

extern int app_file_close(FILE *f);
extern FILE* app_file_open(const char *filename, const char *mode);
extern void app_print(const char* fmt_string, ...);

extern int app_get_resdir(char *path, size_t path_max);
extern int app_get_locale(char *locale, size_t locale_max);
extern int app_get_datadir(const char *app_id, char *path, size_t path_max);

#endif // APP_H