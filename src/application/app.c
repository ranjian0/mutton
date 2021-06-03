#include "application/app.h"

double app_get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}

#if defined(PLATFORM_ANDROID)
#include "app_android.c"
#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#include "app_desktop.c"
#else
#error Unsupported Platform
#endif
