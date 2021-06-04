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

app_t get_app();
void set_app(app_t _app);

int app_get_width();
int app_get_height();

double app_get_time();
void* app_get_userdata();

int app_file_close(FILE *f);
FILE* app_file_open(const char *filename, const char *mode);

int app_get_resdir(char *path, size_t path_max);
int app_get_locale(char *locale, size_t locale_max);
int app_get_datadir(const char *app_id, char *path, size_t path_max);

#if defined(PLATFORM_ANDROID)
#define app_print(...) __android_log_print(ANDROID_LOG_INFO, app.window_title, __VA_ARGS__)
#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#define app_print(...) printf(__VA_ARGS__)
#else
#define app_print(...)
#endif


#endif // APP_H