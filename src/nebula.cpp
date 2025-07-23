#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <Window/window.h>
#include <Graphics/Shader.h>

void getGLVersionInfo();
void getVertexShaderInfo();
void cleanup(nebula::window::Window *window);

// cmake . -B build
// cmake --build build
// build\Debug\nebula

int main() {

    nebula::window::Window *window = new nebula::window::Window();
    window->setWindow();
    
    int width = window->getWidth();
    int height = window->getHeight();

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing GLAD", nullptr);
        cleanup(window);
        return 1;
    }
    
    getGLVersionInfo();
    getVertexShaderInfo();

    float vertices[] = {
    //    x      y     z     r     g     b
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top 
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    nebula::graphics::Shader *shader = new nebula::graphics::Shader("resources/shaders/vertexShader.vs", "resources/shaders/fragShader.fs");
    if (!shader->getId()) {
        delete shader;
        cleanup(window);
        return 1;
    }

    unsigned int ebo {};
    glGenBuffers(1, &ebo);

    unsigned int vao {};
    glGenVertexArrays(1, &vao);

    unsigned int vbo {};
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

        shader->use();

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window->swapBuffers();

    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    cleanup(window);
    return 0;
}

void getGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

void getVertexShaderInfo() {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << "\n";
}

void cleanup(nebula::window::Window *window) {
    delete window;
    SDL_Log("%s", "CLEANUP");
    SDL_Quit();
}
