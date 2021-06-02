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


static void void_func() {}
static app_t app = {
    .init = void_func,
    .update = void_func,
    .shutdown = void_func,
    .window_title = "Mutton Application",
    .window_width = 500,
    .window_height = 500,
};

/*
 * Entry point function mutton applications, must be defined in all applications
 * and must return an app object
 */
app_t mutton_main();

static app_t mutton_get_app() { return app; }
static int mutton_get_width() { return app.window_width; }
static int mutton_get_height() { return app.window_height; }
static void* mutton_get_userdata() { return app.user_data; }

/* Platform specific */
#if defined(PLATFORM_WINDOWS) 
#  if !defined(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  include <Shlobj.h>
#  pragma comment(lib, "Shell32.lib")
#  pragma comment(lib, "Ole32.lib")
#  include <stdlib.h> /* wcstombs_s */
#  if !defined(PATH_MAX)
#    define PATH_MAX MAX_PATH
#  endif
#else
#  include <limits.h> /* PATH_MAX */
#endif

#if defined(PLATFORM_IOS)
#  include <TargetConditionals.h>
#  include <CoreFoundation/CoreFoundation.h>
#  include <objc/objc.h>
#  include <objc/runtime.h>
#  include <objc/message.h>
#  include <objc/NSObjCRuntime.h>
#  include <sys/stat.h> // mkdir
#  ifdef __cplusplus
     extern "C"
#  else
     extern
#  endif
     id NSSearchPathForDirectoriesInDomains(NSUInteger directory, NSUInteger domainMask,
                                            BOOL expandTilde);
#  if defined(__OBJC__) && __has_feature(objc_arc)
#    define FC_AUTORELEASEPOOL_BEGIN @autoreleasepool {
#    define FC_AUTORELEASEPOOL_END }
#  else
#    define FC_MSG_SEND ((id (*)(id, SEL))objc_msgSend)
#    define FC_AUTORELEASEPOOL_BEGIN { \
         id autoreleasePool = FC_MSG_SEND(FC_MSG_SEND((id)objc_getClass("NSAutoreleasePool"), \
             sel_registerName("alloc")), sel_registerName("init"));
#    define FC_AUTORELEASEPOOL_END \
         FC_MSG_SEND(autoreleasePool, sel_registerName("release")); }
#  endif
#elif defined(PLATFORM_ANDROID)
#  include <android/asset_manager.h>
#  include <android/log.h>
#  include <android/native_activity.h>
#  include <jni.h>
#  include <pthread.h>
#  include <string.h>
static JNIEnv *_fc_jnienv(JavaVM *vm);
#elif defined(PLATFORM_LINUX)
#  include <locale.h>
#  include <string.h>
#  include <errno.h>
#  include <unistd.h> // readlink
#  include <sys/stat.h> // mkdir
#endif

#if defined(PLATFORM_WINDOWS)
#  define FC_DIRECTORY_SEPARATOR '\\'
#else
#  define FC_DIRECTORY_SEPARATOR '/'
#endif


double mutton_get_time();
int mutton_get_resdir(char *path, size_t path_max);
int mutton_get_locale(char *locale, size_t locale_max);
int mutton_get_datadir(const char *app_id, char *path, size_t path_max);


#endif // MUTTON_H