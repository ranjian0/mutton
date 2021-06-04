#include "application/app.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow *win, int button, int action, int mod);
static void scroll_callback(GLFWwindow *win, double dx, double dy);
static void cursorpos_callback(GLFWwindow *win, double xpos, double ypos);
static void resize_callback(GLFWwindow *win, int w, int h);
static void error_callback(int error, const char* description);

int main(int argv, char** argc) {
    app = app_main();

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(app.window_width, app.window_height, app.window_title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window\n");
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    if(app.init) app.init();
    while(!glfwWindowShouldClose(window)) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        app.window_width = width;
        app.window_height = height;
        glViewport(0, 0, width, height);

        if(app.update) app.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if(app.shutdown) app.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(app.event.key) {
        KeyEvent ev = {.key=key, .action=action, .mod=mods};
        app.event.key(ev);
    }
     
}

void mouse_callback(GLFWwindow *win, int button, int action, int mod) {
    if(app.event.mouse) {
        MouseEvent ev = {.button=button, .action=action, .mod=mod};
        app.event.mouse(ev);
    }

}

void scroll_callback(GLFWwindow *win, double dx, double dy) {
    if(app.event.scroll) {
        app_print("%f, %f\n", dx, dy);
        MouseScrollEvent ev = {.dx=dx, .dy=dy};
        app.event.scroll(ev);
    }
}

void cursorpos_callback(GLFWwindow *win, double px, double py) {
    app_print("%f, %f\n", px, py);
    if(app.event.move) {
        MouseMoveEvent ev = {.xpos=px, .ypos=py};
        app.event.move(ev);
    }
}

void resize_callback(GLFWwindow *win, int w, int h) {
    if(app.event.resize) {
        ResizeEvent ev = {.width=w, .height=h};
        app.event.resize(ev);
    }
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

