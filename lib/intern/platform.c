#include "defs.h"

#if defined(PLATFORM_ANDROID)
#   include "platform_android.c"
#else
#   include "platform_desktop.c"
#endif
