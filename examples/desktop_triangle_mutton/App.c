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

    printf("%f\n", mutton_get_time());
    char resdir[1024];
    mutton_get_resdir(resdir, 1024);
    printf("%s\n", resdir);

    char locale[1024];
    mutton_get_locale(locale, 1024);
    printf("%s\n", locale);

    char datadir[1024];
    mutton_get_datadir("DeskApp", datadir, 1024);
    printf("%s\n", datadir);


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

void on_key(int key, bool pressed, int mod) {
    if(pressed)
        printf("Key pressed %d\n", key);
    else
        printf("Key released %d\n", key);
}

void on_mouse(int button, bool pressed, int mod) {
    if(pressed)
        printf("mouse pressed %d\n", button);
    else
        printf("mouse released %d\n", button);
}

void on_scroll(double dx, double dy) {
    printf("Mouse Scrolled (%f, %f)\n", dx, dy);
}

void on_move(double xpos, double ypos) {
    printf("Mouse move (%f, %f)\n", xpos, ypos);
}

app_t mutton_main() {
    return (app_t){
        .window_title = "Sample Game",
        .window_width = 800,
        .window_height = 600,
        .init = my_init,
        .update = my_update,
        .shutdown = my_shutdown,
        .event.key = on_key,
        .event.mouse = on_mouse,
        .event.scroll = on_scroll,
        .event.move = on_move
    };
}
