#ifndef PLATFORM_H
#define PLATFORM_H

#include "utils/Defs.h"

#if defined(PLATFORM_ANDROID)
#   include "input_android.h"
#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#   include "input_desktop.h"
#else 
#error Unsupported Platform
#endif


#endif