#ifndef DEFS_H
#define DEFS_H

// PLATFORM DEFINES
#if defined(__ANDROID__)
    #define PLATFORM_ANDROID
#elif defined(__APPLE__)
    #define PLATFORM_APPLE
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(_WIN32) 
    #define PLATFORM_WINDOWS
#else
    #error Unknown platform
#endif

// COMMON INCLUDES
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef PLATFORM_WINDOWS
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include "ft2build.h"
#include FT_FREETYPE_H


// PLATFORM SPECIFIC INCLUDES
#ifdef PLATFORM_ANDROID
#include "glfm.h"
#define FILE_COMPAT_ANDROID_ACTIVITY glfmAndroidGetActivity()
#else
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif


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

#if defined(PLATFORM_APPLE)
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
#  include <unistd.h> // readlink
#  include <sys/stat.h> // mkdir
#endif

#if defined(PLATFORM_WINDOWS)
#  define FC_DIRECTORY_SEPARATOR '\\'
#else
#  define FC_DIRECTORY_SEPARATOR '/'
#endif

#define UNUSED(x) (void)(x)

#if defined(PLATFORM_WINDOWS)
// https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

// MSVC defines this in winsock2.h!?
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

#endif

#endif // DEFS_H