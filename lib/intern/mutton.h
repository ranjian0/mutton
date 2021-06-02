#ifndef MUTTON_H
#define MUTTON_H

#include "defs.h"
#include "event.h"

#if defined(PLATFORM_ANDROID)
#   include "input/input_android.h"
#else
#   include "input/input_desktop.h"
#endif

typedef struct app_t 
{
    void (* init)();
    void (* update)();
    void (* shutdown)();
    const char* window_title;
    int window_width;
    int window_height;
    void* user_data;

    event_t event;
} app_t;

// Global App object
static void void_func() {}
static app_t app = {
    .init = void_func,
    .update = void_func,
    .shutdown = void_func,
    .window_title = "MuttonApp",
    .window_width = 500,
    .window_height = 500,
};

/*
 * Entry point function for mutton applications. 
 * Must be defined in all applications and must return an app object
 * 
 * EXAMPLE App.c
 * 
 * app_t mutton_main() {
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
app_t mutton_main();

static app_t mutton_get_app() { return app; }
static int mutton_get_width() { return app.window_width; }
static int mutton_get_height() { return app.window_height; }
static void* mutton_get_userdata() { return app.user_data; }
static double mutton_get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}


int mutton_get_resdir(char *path, size_t path_max);
int mutton_get_locale(char *locale, size_t locale_max);
int mutton_get_datadir(const char *app_id, char *path, size_t path_max);


#endif // MUTTON_H