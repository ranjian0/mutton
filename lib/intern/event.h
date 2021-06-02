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

typedef struct event_t
{
    void (*resize)(ResizeEvent ev);

    void (*key)(KeyEvent ev);
    void (*mouse)(MouseEvent ev);
    void (*scroll)(MouseScrollEvent ev);
    void (*move)(MouseMoveEvent ev);

    void (*touch)(TouchEvent ev);

} event_t;

#endif // EVENT_H