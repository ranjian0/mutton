#include "application/app.h"

// Empty app handler functions
void void_func() {}
void no_resize(ResizeEvent ev) {}
void no_key(KeyEvent ev) {}
void no_mouse(MouseEvent ev) {}
void no_scroll(MouseScrollEvent ev) {}
void no_move(MouseMoveEvent ev) {}
void no_touch(TouchEvent ev) {}
void no_sensor(SensorEvent ev) {}

// Default app definitions
app_t app = {
    .init = void_func,
    .update = void_func,
    .shutdown = void_func,
    .window_title = "App",
    .window_width = 800,
    .window_height = 600,
    .event.resize = no_resize,
    .event.key = no_key,
    .event.mouse = no_mouse,
    .event.scroll = no_scroll,
    .event.move = no_move,
    .event.touch = no_touch,
    .event.sensor = no_sensor,        
};


#if defined(PLATFORM_ANDROID)
#include "app_android.c"
#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#include "app_desktop.c"
#else
#error Unsupported Platform
#endif
