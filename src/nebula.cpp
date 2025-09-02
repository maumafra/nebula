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
    nebula::window::Window& window;
    nebula::graphics::Graphics& graphics;
    nebula::ecs::World& world;
};

void cleanup(State &s) {
    SDL_Log("%s", "CLEANUP");
    SDL_Quit();
}

bool setup(State &s) {
    s.window.setWindow();

    if (!s.graphics.initialize()) {
        SDL_Log("%s", "ERROR INITIALIZING GRAPHICS");
        return false;
    }

    auto boxSprite = s.graphics.newSprite("resources/textures/container.jpg");
    auto ninaSprite = s.graphics.newSprite("resources/textures/Nina.png");

    auto box = s.world.spawn();
    s.world.addComponent(box, Position{400.0f, 300.0f});
    s.world.addComponent(box, Sprite{boxSprite});
    s.world.addComponent(box, Scale{0.5f, 0.5f});

    auto whiteSquare = s.world.spawn();
    s.world.addComponent(whiteSquare, Position{10.0f, 10.0f});
    s.world.addComponent(whiteSquare, Quad{100.0f, 200.0f});

    auto nina = s.world.spawn();
    s.world.addComponent(nina, Position{0.0f, 300.0f});
    s.world.addComponent(nina, Sprite{ninaSprite});
    s.world.addComponent(nina, Scale{0.5f, 0.5f});

    return true;
}

void update(State& s) {
}

void draw(State& s) {
    s.graphics.beginScene(&s.world);

    auto entities = s.world.getEntitiesWith<Position>();

    for (auto entity : entities) {
        s.graphics.draw(entity);
    }

    s.graphics.endScene();
    s.window.swapBuffers();
}

// cmake . -B build
// cmake --build build
// build\Debug\nebula

int main() {
    State s = {
        nebula::window::Window(),
        nebula::graphics::Graphics(800, 600),
        nebula::ecs::World()
    };
    if (!setup(s)) {
        cleanup(s);
        return 1;
    }
    update(s);
    draw(s);
    update(s);
    draw(s);

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
    }
    cleanup(s);
    return 0;
}
