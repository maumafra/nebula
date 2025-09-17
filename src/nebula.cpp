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
#include "modules/scripts/NebulaSetup.hpp"

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

enum RunAction {
    RUN,
    QUIT
};

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

    auto whiteSquare = s.world.spawn();
    s.world.addComponent(whiteSquare, Position{10.0f, 10.0f});
    s.world.addComponent(whiteSquare, Quad{100.0f, 200.0f});

    return true;
}

void update(State& s) {
    auto e = s.world.getEntitiesWith<Position>()[0];
    Position* pos = s.world.getComponent<Position>(e);
    pos->x += 10.0f;
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

int main2() {
    State s = {
        nebula::window::Window(),
        nebula::graphics::Graphics(800, 600),
        nebula::ecs::World()
    };
    if (!setup(s)) {
        cleanup(s);
        return 1;
    }

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
        update(s);
        draw(s);
    }
    cleanup(s);
    return 0;
}

static void nlua_PackagePreload(lua_State *L, lua_CFunction func, const char *packName) {
    // standard lua table "package"
    lua_getglobal(L, "package"); // places table at the top of stack
    lua_getfield(L, -1, "preload"); // retrieves the "preload" field from the table at the top of stack (-1)
    lua_pushcfunction(L, func);
    lua_setfield(L, -2, packName); // saves func on package at packName (key)
    lua_pop(L, 2); // pops 2 elements from the stack
}


static RunAction runNebula(int argc, char **argv, int &mainReturn) {
    // TODO args handling

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    nlua_PackagePreload(L, nlua_nebula, "nebula");
}

int main(int argc, char **argv) {
    int mainReturn = 0;
    RunAction action = RUN;

    do {
        action = runNebula(argc, argv, mainReturn);
    } while (action != QUIT);

    SDL_Quit();
    return mainReturn;
}