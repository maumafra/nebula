#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "../Ecs/World.hpp"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace nebula {
    namespace graphics {

class Graphics {
public:
    Graphics(int width, int height);
    ~Graphics();
    bool initialize();

    void beginScene(ecs::World* world);
    void endScene();
    void draw(ecs::EntityId entity);

    Texture* newSprite(std::string path);

private:
    Shader* defaultShader;
    Camera* defaultCamera;
    Renderer* renderer;

    int width, height;
    
    void getGLVersionInfo();
    void getVertexShaderInfo();
    
    std::unordered_map<std::string, Texture*> textures;
}; 

}// graphics
}// nebula