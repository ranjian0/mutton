#include "mutton.h"

void app_init() {
    printf("%f\n", mutton_get_time());
    char resdir[1024];
    mutton_get_resdir(resdir, 1024);
    printf("%s\n", resdir);

    char locale[1024];
    mutton_get_locale(locale, 1024);
    printf("%s\n", locale);

    char datadir[1024];
    mutton_get_datadir("App", datadir, 1024);
    printf("%s\n", datadir);

}

void app_update() {
    glViewport(0, 0, mutton_get_width(), mutton_get_height());
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


}

void app_shutdown() {

}

void on_touch(TouchEvent ev) {
    printf("Touch Event at %f, %f\n", ev.x, ev.y);
}

app_t mutton_main() {
    return (app_t) {
        .init = app_init,
        .update = app_update,
        .shutdown = app_shutdown,
        .event.touch = on_touch
    };
}