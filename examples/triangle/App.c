#include "mutton.h"

typedef struct Vertex
{
    float pos[3];
    float col[3];
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

static const char* vertex_shader_text =
"#version 330\n"

"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

GLuint program;
GLuint vertex_array;

void my_init() {
    // Do your initialization
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");


    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    app_print("%f\n", app_get_time());
    char resdir[1024];
    app_get_resdir(resdir, 1024);
    app_print("ResDir: %s\n", resdir);

    char locale[1024];
    app_get_locale(locale, 1024);
    app_print("Locale: %s\n", locale);

    char datadir[1024];
    app_get_datadir("DeskApp", datadir, 1024);
    app_print("Datadir: %s\n", datadir);

}

void my_update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.4, 1.0);

    glUseProgram(program);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void my_shutdown() {
    glDeleteProgram(program);

}

void on_key(KeyEvent ev) {
    if (ev.key == KEY_ESCAPE) {
        app_print("Key pressed space\n");
    }
    if(ev.action == ACTION_REPEAT) {
        app_print("key repeating..\n");
    }
}

void on_mouse(MouseEvent ev) {
    if(ev.button == MOUSE_BUTTON_LEFT) {
        app_print("mouse pressed left\n");
    }
    if(ev.action == ACTION_RELEASE) {
        app_print("button released..\n");
    }
}

void on_scroll(MouseScrollEvent ev) {
    app_print("Mouse Scrolled (%f, %f)\n", ev.dx, ev.dy);
}

void on_move(MouseMoveEvent ev) {
    app_print("Mouse move (%f, %f)\n", ev.xpos, ev.ypos);
}

void on_touch(TouchEvent ev) {
    app_print("Touch event at (%f, %f)\n", ev.x, ev.y);
}

app_t app_main() {
    return (app_t){
        .window_title = "Triangle",
        .window_width = 800,
        .window_height = 600,
        .init = my_init,
        .update = my_update,
        .shutdown = my_shutdown,
        .event.key = on_key,
        .event.mouse = on_mouse,
        .event.scroll = on_scroll,
        .event.move = on_move,
        .event.touch = on_touch
    };
}
