#ifndef PLATFORM_H
#define PLATFORM_H

#include "utils/Defs.h"

#include "input_desktop.h"

// if we are on android, similar inputs are redefined
// doing allows building multiplatform apps in one codebase
#if defined(PLATFORM_ANDROID)
#include "input_android.h"
#endif


#endif