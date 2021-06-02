#ifndef MUTTON_H
#define MUTTON_H

// PLATFORM DEFINES
#if defined(__ANDROID__)
    #define PLATFORM_ANDROID
#elif defined(__APPLE__)
    #define PLATFORM_IOS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(_WIN32) 
    #define PLATFORM_WINDOWS
#else
    #error Unknown platform
#endif

// COMMON MACRO DEFINES

#define MakeString(s) #s

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ft2build.h"
#include FT_FREETYPE_H

// APPLICATION DEFINITION AND PLATFORM HOOKS
#ifdef PLATFORM_ANDROID
#include "glfm.h"
#else
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

typedef struct event_t
{
    void (*key)(int value, bool pressed, int modifiers);
    void (*mouse)(int button, bool pressed, int modifiers);
    void (*scroll)(double xoffset, double yoffset);
    void (*move)(double xpos, double ypos);

    void (*touch)(int x, int y, bool state);

} event_t;


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


void void_func() {}
static app_t app = {
    .init = void_func,
    .update = void_func,
    .shutdown = void_func,
    .window_title = "Mutton Application",
    .window_width = 500,
    .window_height = 500,
};

app_t mutton_main();
static app_t mutton_get_app() { return app; }
static int mutton_get_width() { return app.window_width; }
static int mutton_get_height() { return app.window_height; }
static void* mutton_get_userdata() { return app.user_data; }

// Platform specific
double mutton_get_time();


// Define main hooks for different platforms


#endif // MUTTON_H