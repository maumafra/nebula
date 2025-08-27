#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <Window/window.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/Graphics.h>
#include <Ecs/World.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct State {
    nebula::window::Window* window = nullptr;
    nebula::graphics::Graphics* graphics = nullptr;
};

void cleanup(State* s) {
    delete s->window;
    delete s->graphics;
    SDL_Log("%s", "CLEANUP");
    SDL_Quit();
}

bool setup(State* s) {
    nebula::window::Window *window = new nebula::window::Window();
    window->setWindow();

    int width = window->getWidth();
    int height = window->getHeight();

    s->window = window;

    nebula::graphics::Graphics *graphics = new nebula::graphics::Graphics(width, height);
    if (!graphics->initialize()) {
        SDL_Log("%s", "ERROR INITIALIZING GRAPHICS");
        return false;
    }

    s->graphics = graphics;
    
    return true;
}

// cmake . -B build
// cmake --build build
// build\Debug\nebula

int main() {
    nebula::ecs::World ecs;
    State s = State();
    if (!setup(&s)) {
        cleanup(&s);
        return 1;
    }

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
    };

    unsigned int indices[] = {
        1, 2, 3,
        0, 1, 3
    };

    nebula::graphics::Shader* shader = new nebula::graphics::Shader("resources/shaders/vertexShader.vert", "resources/shaders/fragShader.frag");
    // maybe shader function to validate? like shader->validate();
    if (!shader->getId()) {
        delete shader;
        cleanup(&s);
        return 1;
    }

    unsigned int ebo {};
    glGenBuffers(1, &ebo);

    unsigned int vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    nebula::graphics::Texture* texture = new nebula::graphics::Texture("resources/textures/container.jpg");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    bool run = true;
    while (run) {

        SDL_Event event{0};
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    run = false;
                    break;
                }
            }
        }

        s.graphics->setupDraw();
        texture->use();
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        shader->use();

        unsigned int transformLoc = glGetUniformLocation(shader->getId(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        s.window->swapBuffers();

    }
    delete texture;
    delete shader;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    cleanup(&s);
    return 0;
}
