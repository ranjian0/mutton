#ifndef EVENT_H
#define EVENT_H

typedef struct ResizeEvent {
    int width, height;
} ResizeEvent;

typedef struct KeyEvent {
    int key;
    int action;
    int mod;
} KeyEvent;

typedef struct MouseEvent {
    int button;
    int action;
    int mod;
} MouseEvent;

typedef struct MouseScrollEvent {
    double dx, dy; 
} MouseScrollEvent;

typedef struct MouseMoveEvent {
    double xpos, ypos;
} MouseMoveEvent;

typedef struct TouchEvent {
    int id;
    int phase;
    double x, y;
} TouchEvent;

typedef struct SensorEvent {
    int type;
    double timestamp;

    union {
        struct {
            double x, y, z;
        } vector;
        struct {
            double m00, m01, m02;
            double m10, m11, m12;
            double m20, m21, m22;
        } matrix;
    };

} SensorEvent;

typedef struct event_t
{
    void (*resize)(ResizeEvent ev);

    void (*key)(KeyEvent ev);
    void (*mouse)(MouseEvent ev);
    void (*scroll)(MouseScrollEvent ev);
    void (*move)(MouseMoveEvent ev);

    void (*touch)(TouchEvent ev);
    void (*sensor)(SensorEvent ev);

} event_t;

#endif // EVENT_H