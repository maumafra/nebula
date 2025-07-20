#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <Window/window.h>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main() {\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main() {\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" //orange-ish color
                                   "}\0";

void getGLVersionInfo();
unsigned int setupShaders();
void cleanup();

// cmake . -B build
// cmake --build build
// build\Debug\nebula

int main() {

    nebula::window::Window window = nebula::window::Window();
    window.setWindow();
    
    int width = window.getWidth();
    int height = window.getHeight();

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing GLAD", nullptr);
        window.close();
        cleanup();
        return 1;
    }
    
    getGLVersionInfo();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int shaderProgram = setupShaders();
    if (!shaderProgram) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error compilating vertex shader", nullptr);
        window.close();
        cleanup();
        return 1;
    }

    unsigned int vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo {};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bool run = true;
    while (run) {

        glViewport(0, 0, width, height);

        SDL_Event event{0};
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    run = false;
                    break;
                }
            }
        }

        // rendering
        // (state-setting function)
        glClearColor(0.07f, 0.0f, 0.125f, 1.0f);
        // (state-using function) fills the color buffer with the color configured by glClearColor
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();

    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
    cleanup();
    return 0;
}

void getGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

unsigned int setupShaders() {
    int success {};
    char infoLog[512] {};

    unsigned int vertexShader {};
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        SDL_Log("%s,%s","ERROR::SHADER::VERTEX::COMPILATION_FAILED", infoLog);
        return success;
    }

    unsigned int fragmentShader {};
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        SDL_Log("%s,%s","ERROR::SHADER::FRAGMENT::COMPILATION_FAILED", infoLog);
        return success;
    }

    unsigned int shaderProgram {};
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        SDL_Log("%s,%s","ERROR::SHADER::PROGRAM::COMPILATION_FAILED", infoLog);
        return success;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void cleanup() {
    SDL_Log("%s", "CLEANUP");
    SDL_Quit();
}
