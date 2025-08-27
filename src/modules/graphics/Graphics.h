#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "Shader.h"

#include <iostream>
#include <vector>

namespace nebula {
    namespace graphics {

class Graphics {
public:
    Graphics(int width, int height);
    ~Graphics();
    bool initialize();
    void setupDraw();

private:
    unsigned int vao;
    Shader* defaultShader;
    int width;
    int height;
    void getGLVersionInfo();
    void getVertexShaderInfo();
    
    std::vector<GLuint> textures;
}; 

}// graphics
}// nebula

#endif