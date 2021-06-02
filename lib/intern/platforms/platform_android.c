#include "../mutton.h"
#include "glfm.h"

static bool needs_redraw = true;

static void onFrame(GLFMDisplay *display);
static void onSurfaceCreated(GLFMDisplay *display, int width, int height);
static void onSurfaceRefresh(GLFMDisplay *display);
static void onSurfaceDestroyed(GLFMDisplay *display);
static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y);
static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers);
static void onSensor(GLFMDisplay *display, GLFMSensorEvent sev);

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
    glfmSetSensorFunc(display, GLFMSensorGyroscope,onSensor);
    glfmSetSensorFunc(display, GLFMSensorAccelerometer,onSensor);
    glfmSetSensorFunc(display, GLFMSensorMagnetometer,onSensor);
    glfmSetSensorFunc(display, GLFMSensorRotationMatrix,onSensor);
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

static void onSensor(GLFMDisplay *display, GLFMSensorEvent sev) {
    app_t this_app = mutton_get_app();
    SensorEvent ev;
    ev.type = sev.sensor;
    ev.timestamp = sev.timestamp;

    if(sev.sensor == GLFMSensorRotationMatrix) {
        ev.matrix.m00 = sev.matrix.m00;
        ev.matrix.m10 = sev.matrix.m10;
        ev.matrix.m20 = sev.matrix.m20;
        ev.matrix.m01 = sev.matrix.m01;
        ev.matrix.m11 = sev.matrix.m11;
        ev.matrix.m21 = sev.matrix.m21;
        ev.matrix.m02 = sev.matrix.m02;
        ev.matrix.m12 = sev.matrix.m12;
        ev.matrix.m22 = sev.matrix.m22;        
    } else {
        ev.vector.x = sev.vector.x;
        ev.vector.y = sev.vector.y;
        ev.vector.z = sev.vector.z;
    }
    this_app.event.sensor(ev);
}