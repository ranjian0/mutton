#include "defs.h"

#if defined(PLATFORM_ANDROID)
#   include "platforms/platform_android.c"
#else
#   include "platforms/platform_desktop.c"
#endif
