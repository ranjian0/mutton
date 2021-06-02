#include "mutton.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow *win, int button, int action, int mod);
static void scroll_callback(GLFWwindow *win, double dx, double dy);
static void cursorpos_callback(GLFWwindow *win, double xpos, double ypos);
static void resize_callback(GLFWwindow *win, int w, int h);
static void error_callback(int error, const char* description);

int main(int argv, char** argc) {
    app = mutton_main();

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

    app.init();
    while(!glfwWindowShouldClose(window)) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        app.window_width = width;
        app.window_height = height;
        glViewport(0, 0, width, height);

        app.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    app_t this_app = mutton_get_app();
    KeyEvent ev = {.key=key, .action=action, .mod=mods};
    this_app.event.key(ev);
     
}

static void mouse_callback(GLFWwindow *win, int button, int action, int mod) {
    app_t this_app = mutton_get_app();
    MouseEvent ev = {.button=button, .action=action, .mod=mod};
    this_app.event.mouse(ev);

}

static void scroll_callback(GLFWwindow *win, double dx, double dy) {
    app_t this_app = mutton_get_app();
    MouseScrollEvent ev = {.dx=dx, .dy=dy};
    this_app.event.scroll(ev);
}

static void cursorpos_callback(GLFWwindow *win, double xpos, double ypos) {
    app_t this_app = mutton_get_app();
    MouseMoveEvent ev = {.xpos=xpos, ypos=ypos};
    this_app.event.move(ev);
}

static void resize_callback(GLFWwindow *win, int w, int h) {
    app_t this_app = mutton_get_app();
    ResizeEvent ev = {.width=w, .height=h};
    this_app.event.resize(ev);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

