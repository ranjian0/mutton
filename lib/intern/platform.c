#include "mutton.h"

#ifdef PLATFORM_ANDROID

#else // Dekstop Applications
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

double mutton_get_time() { return glfwGetTime(); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    app_t this_app = mutton_get_app();
    if(action == GLFW_PRESS)
        this_app.event.key(key, true, mods);
    else if (action == GLFW_RELEASE)
        this_app.event.key(key, false, mods);
     
}

static void mouse_callback(GLFWwindow *win, int button, int action, int mod) {
    app_t this_app = mutton_get_app();
    if(action == GLFW_PRESS)
        this_app.event.mouse(button, true, mod);
    else if(action == GLFW_RELEASE)
        this_app.event.mouse(button, false, mod);

}

static void scroll_callback(GLFWwindow *win, double dx, double dy) {
    app_t this_app = mutton_get_app();
    this_app.event.scroll(dx, dy);
}

static void cursorpos_callback(GLFWwindow *win, double xpos, double ypos) {
    app_t this_app = mutton_get_app();
    this_app.event.move(xpos, ypos);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


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

#endif // PLATFORM ANDROID
