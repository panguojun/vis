#pragma once
// ***********************************************************************************************
void checkGlShader(GLuint shader, const char* file, int line)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int loglen;
        char logbuffer[1000];
        glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
        fprintf(stderr, "OpenGL Shader Compile Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
    }
    else {
        int loglen;
        char logbuffer[1000];
        glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
        if (loglen > 0) {
            fprintf(stderr, "OpenGL Shader Compile OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
        }
    }
}

void checkGlProgram(GLuint prog, const char* file, int line)
{
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int loglen;
        char logbuffer[1000];
        glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
        fprintf(stderr, "OpenGL Program Linker Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
    }
    else {
        int loglen;
        char logbuffer[1000];
        glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
        if (loglen > 0) {
            fprintf(stderr, "OpenGL Program Link OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
        }
        glValidateProgram(prog);
        glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
        if (loglen > 0) {
            fprintf(stderr, "OpenGL Program Validation results at %s:%d:\n%.*s", file, line, loglen, logbuffer);
        }
    }
}

// 读取shader文件内容
string read_shader_file(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        throw runtime_error("Failed to open shader file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 编译shader
GLuint compile_shader(GLenum type, const string& source)
{
    GLuint shader = glCreateShader(type);
    const char* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        throw runtime_error("Failed to compile shader:\n" + string(log));
    }
    return shader;
}

// 加载shader
GLuint load_shader(const string& vertex_shader_filename, const string& fragment_shader_filename)
{
    // 读取shader文件内容
    string vertex_shader_source = read_shader_file(vertex_shader_filename);
    string fragment_shader_source = read_shader_file(fragment_shader_filename);

    // 编译并链接shader
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        throw runtime_error("Failed to link shader program:\n" + string(log));
    }

    // 删除shader对象
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

/*
{
    GLenum glew_err = glewInit();
    if (glew_err != GLEW_OK) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glew_err));
        return;
    }
    const char* vs_source =
        "attribute float y;\n"
        "varying float vy;\n"
        "void main() {\n"
        "	gl_FrontColor = gl_Color;\n"
        "	gl_Position = ftransform();\n"
        "	vy = y;\n"
        "}\n";
    const char* fs_source =
        "#extension GL_EXT_gpu_shader4 : enable\n"
        "varying float vy;\n"
        "void main() {\n"
        "	if (int(vy*15.0) % 2 == 1)\n"
        "		discard;\n"
        "	gl_FragColor = gl_Color;\n"
        "}\n";

    prog = glCreateProgram();

    GLuint vs = (glCreateShader(GL_VERTEX_SHADER));
    (glShaderSource(vs, 1, (const GLchar**)&vs_source, NULL));
    (glCompileShader(vs));
    checkGlShader(vs, "(vs) " __FILE__, __LINE__);
    (glAttachShader(prog, vs));

    GLuint fs = (glCreateShader(GL_FRAGMENT_SHADER));
    (glShaderSource(fs, 1, (const GLchar**)&fs_source, NULL));
    (glCompileShader(fs));
    checkGlShader(fs, "(fs) " __FILE__, __LINE__);
    (glAttachShader(prog, fs));

    (glLinkProgram(prog));
    checkGlProgram(prog, __FILE__, __LINE__);
}

int main1(int argc, char** argv)
{
    // 初始化OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // 加载shader
    GLuint shader_program = load_shader(
        "C:/Users/18858/Documents/_LAB/glRender/glRender/shader/vertex_shader.glsl",
        "C:/Users/18858/Documents/_LAB/glRender/glRender/shader/fragment_shader.glsl");
    glUseProgram(shader_program);

    // init();
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glfwTerminate();
}*/