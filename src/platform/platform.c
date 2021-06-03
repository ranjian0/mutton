#include "utils/Defs.h"

#if defined(PLATFORM_ANDROID)
#   include "platform_android.c"
#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#   include "platform_desktop.c"
#else
#error unsupported platform
#endif