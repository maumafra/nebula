#include "Graphics.h"

namespace nebula {
    namespace graphics {


Graphics::Graphics(int width, int height) 
            : vao(NULL)
            , defaultShader(nullptr)
            , width(width)
            , height(height) {
    this->initialize();
    this->getGLVersionInfo();
    this->getVertexShaderInfo();
}
Graphics::~Graphics() {
    std::cout << "DELETE GRAPHICS" << "\n";
    glDeleteVertexArrays(1, &vao);
}

bool Graphics::initialize() {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        return false;
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return true;
}

void Graphics::setupDraw() {
    glViewport(0, 0, width, height);

    // (state-setting function)
    glClearColor(0.07f, 0.0f, 0.125f, 1.0f);
    // (state-using function) fills the color buffer with the color configured by glClearColor
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
}

void Graphics::getGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

void Graphics::getVertexShaderInfo() {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << "\n";
}

}// graphics
}// nebula
