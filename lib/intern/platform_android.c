#include "mutton.h"
#include "glfm.h"

static bool needs_redraw = true;

static void onFrame(GLFMDisplay *display);
static void onSurfaceCreated(GLFMDisplay *display, int width, int height);
static void onSurfaceRefresh(GLFMDisplay *display);
static void onSurfaceDestroyed(GLFMDisplay *display);
static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y);
static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers);

void glfmMain(GLFMDisplay *display) {
    app = mutton_main();

    glfmSetDisplayConfig(display,
                         GLFMRenderingAPIOpenGLES2,
                         GLFMColorFormatRGBA8888,
                         GLFMDepthFormatNone,
                         GLFMStencilFormatNone,
                         GLFMMultisampleNone);
    // glfmSetUserData(display, app);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceRefreshFunc(display, onSurfaceRefresh);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    glfmSetRenderFunc(display, onFrame);
    glfmSetTouchFunc(display, onTouch);
    glfmSetKeyFunc(display, onKey);
}


static void onFrame(GLFMDisplay *display) {
    app_t this_app = mutton_get_app();

    if(needs_redraw) {
        needs_redraw = false; 

        int width, height;
        glfmGetDisplaySize(display, &width, &height);
        this_app.window_width = width;
        this_app.window_height = height;
        this_app.update();
        glfmSwapBuffers(display);

    }
}

static void onSurfaceCreated(GLFMDisplay *display, int width, int height) {
    app_t this_app = mutton_get_app();
    this_app.init();

}

static void onSurfaceRefresh(GLFMDisplay *display) {
    needs_redraw = true;
}

static void onSurfaceDestroyed(GLFMDisplay *display) {
    app.shutdown();
}

static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y) {
    app_t this_app = mutton_get_app();
    TouchEvent ev = {.id=touch, .phase=phase, .x=x, .y=y};
    this_app.event.touch(ev);
    return true;
}

static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers) {
    app_t this_app = mutton_get_app();
    KeyEvent ev = {.key=keyCode, .action=action, .mod=modifiers};
    this_app.event.key(ev);
    return true;
}

