#include "Graphics.h"

namespace nebula {
    namespace graphics {


Graphics::Graphics(int width, int height)
            : defaultShader(nullptr)
            , defaultCamera(nullptr)
            , renderer(nullptr)
            , width(width)
            , height(height) {
}

Graphics::~Graphics() {
    std::cout << "DELETE GRAPHICS" << "\n";
    delete renderer;
    delete defaultCamera;
    delete defaultShader;
    for (const auto& pair : textures) {
        delete pair.second;
    }
}

bool Graphics::initialize() {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        return false;
    }
    defaultShader = new Shader("resources/shaders/vertexShader.vert", "resources/shaders/fragShader.frag");
    if (!defaultShader->getId()) {
        return false;
    }
    defaultCamera = new Camera(width, height);
    renderer = new Renderer();
    renderer->init();

    this->getGLVersionInfo();
    this->getVertexShaderInfo();

    return true;
}

void Graphics::beginScene(ecs::World* world) {
    glViewport(0, 0, width, height);

    // (state-setting function)
    glClearColor(0.07f, 0.0f, 0.125f, 1.0f);
    // (state-using function) fills the color buffer with the color configured by glClearColor
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    renderer->begin(world, defaultCamera, defaultShader);
}

void Graphics::endScene() {
    renderer->end();
}

Texture* Graphics::newSprite(std::string path) {
    if (textures.count(path) != 0) {
        return textures.at(path);
    }
    Texture* tex = new Texture(path.c_str());
    textures.insert({path, tex});
    return tex;
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

void Graphics::draw(ecs::EntityId entity) {
    renderer->drawEntity(entity);
}

}// graphics
}// nebula
