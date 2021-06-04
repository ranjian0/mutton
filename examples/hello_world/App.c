#include "mutton.h"

void app_init();
void app_update();
void app_exit();

app_t app_main() {
    return (app_t) {
        .init = app_init,
        .update = app_update,
        .shutdown = app_exit,
        .window_title = "Hello World",
        .window_width = 800,
        .window_height = 600,
    };
}

void app_init() {
    app_print("Hello World\n");
}

void app_update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(.2, .3, .4, 1.0);
}

void app_exit() {
    app_print("Bye World\n");
}
