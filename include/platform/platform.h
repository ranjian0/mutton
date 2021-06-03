#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(PLATFORM_ANDROID)
#   include "input_android.h"
#else
#   include "input_desktop.h"
#endif


#endif