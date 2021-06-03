#ifndef INPUT_ANDROID_H
#define INPUT_ANDROID_H

#define TouchPhaseHover         GLFMTouchPhaseHover
#define TouchPhaseBegan         GLFMTouchPhaseBegan
#define TouchPhaseMoved         GLFMTouchPhaseMoved
#define TouchPhaseEnded         GLFMTouchPhaseEnded
#define TouchPhaseCancelled     GLFMTouchPhaseCancelled

#define KEY_BACKSPACE   0x08
#define KEY_TAB         0x09
#define KEY_ENTER       0x0d
#define KEY_ESCAPE      0x1b
#define KEY_SPACE       0x20
#define KEY_PAGEUP      0x21
#define KEY_PAGEDOWN    0x22
#define KEY_END         0x23
#define KEY_HOME        0x24
#define KEY_LEFT        0x25
#define KEY_UP          0x26
#define KEY_RIGHT       0x27
#define KEY_DOWN        0x28
#define KEY_DELETE      0x2E
#define KEY_NAVBACK     0x1000
#define KEY_NAVMENU     0x1001
#define KEY_NAVSELECT   0x1002
#define KEY_PLAYPAUSE   0x2000

#define MOD_SHIFT  (1 << 0),
#define MOD_CTRL  (1 << 1),
#define MOD_ALT  (1 << 2),
#define MOD_META  (1 << 3),

#define ACTION_PRESSED    GLFMKeyActionPressed
#define ACTION_REPEATED   GLFMKeyActionRepeated
#define ACTION_RELEASED   GLFMKeyActionReleased

#define SENSOR_ACCELEROMETER    GLFMSensorAccelerometer    // Events are a vector in G's
#define SENSOR_MAGNETOMETER     GLFMSensorMagnetometer     // Events are a vector in microteslas
#define SENSOR_GYROSCOPE        GLFMSensorGyroscope        // Events are a vector in radians/sec
#define SENSOR_ROTATIONMATRIX   GLFMSensorRotationMatrix   // Events are a rotation matrix

#endif // INPUT_ANDROID_H