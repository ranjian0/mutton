#include "glfm.h"
#include "application/app.h"

static bool needs_redraw = true;

void onFrame(GLFMDisplay *display);
void onSurfaceCreated(GLFMDisplay *display, int width, int height);
void onSurfaceRefresh(GLFMDisplay *display);
void onSurfaceDestroyed(GLFMDisplay *display);
bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y);
bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers);
void onSensor(GLFMDisplay *display, GLFMSensorEvent sev);

void glfmMain(GLFMDisplay *display) {
    app = app_main();

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


void onFrame(GLFMDisplay *display) {

    if(needs_redraw) {
        needs_redraw = false; 

        int width, height;
        glfmGetDisplaySize(display, &width, &height);
        app.window_width = width;
        app.window_height = height;
        if(app.update) app.update();
        glfmSwapBuffers(display);

    }
}

void onSurfaceCreated(GLFMDisplay *display, int width, int height) {
    if(app.init) app.init();

}

void onSurfaceRefresh(GLFMDisplay *display) {
    needs_redraw = true;
}

void onSurfaceDestroyed(GLFMDisplay *display) {
    if(app.shutdown) app.shutdown();
}

bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y) {
    if(app.event.touch) {
        TouchEvent ev = {.id=touch, .phase=phase, .x=x, .y=y};
        app.event.touch(ev);
    }
    return true;
}

bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers) {
    if(app.event.key) {
        KeyEvent ev = {.key=keyCode, .action=action, .mod=modifiers};
        app.event.key(ev);
    }
    return true;
}

void onSensor(GLFMDisplay *display, GLFMSensorEvent sev) {
    if(app.event.sensor) {
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
        app.event.sensor(ev);
    }
}