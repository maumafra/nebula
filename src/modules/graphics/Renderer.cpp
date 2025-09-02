#include "Renderer.h"
#include <cstdio>

namespace nebula {
    namespace graphics {

Renderer::Renderer() 
    : vertexCount(0), indexCount(0), vertices(nullptr), world(nullptr) {
}

Renderer::~Renderer() {
    delete[] vertices;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

bool Renderer::init() {
    vertices = new Vertex[MAX_VERTICES];

    if (!setupBuffers()) {
        return false;
    }

    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        textureSlots[i] = i;
    }

    return true;
}

bool Renderer::setupBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    std::vector<uint32_t> indices;
    indices.reserve(MAX_INDICES);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_QUADS; i++) {
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);
        offset += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Texture Coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    // Texture Idx
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));

    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);

    //std::vector<uint32_t> indices;
    //indices.reserve(MAX_INDICES);

    //uint32_t offset = 0;
    //for (uint32_t i = 0; i < MAX_QUADS; i++) {
    //    indices.push_back(offset + 0);
    //    indices.push_back(offset + 1);
    //    indices.push_back(offset + 2);
    //    indices.push_back(offset + 2);
    //    indices.push_back(offset + 3);
    //    indices.push_back(offset + 0);
    //    offset += 4;
    //}

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return true;
}

void Renderer::begin(ecs::World* world, Camera* camera, Shader* shader) {
    this->camera = camera;
    this->world = world;
    this->shader = shader;
    vertexCount = 0;
    indexCount = 0;
    textures.clear();
}

void Renderer::end() {
    flush();
}

void Renderer::flush() {
    if (indexCount == 0) {
        return;
    }

    // upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), vertices);

    shader->use();

    // bind textures
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i]->bind();
    }
    glActiveTexture(GL_TEXTURE0);

    // texture uniforms
    for (uint32_t i = 0; i < textures.size(); i++) {
        shader->setInt("uTexture" + std::to_string(i), i);
    }

    // set shader
    shader->setMat4("uView", camera->getView());
    shader->setMat4("uProjection", camera->getProjection());

    // draw
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // reset this batch
    vertexCount = 0;
    indexCount = 0;
    textures.clear();
}

void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, Texture* texture) {
    if (indexCount >= MAX_INDICES) {
        flush();
    }

    float textureIndex = -1.0f;

    if (texture) {
        // check if texture is already in batch
        for (uint32_t i = 0; i < textures.size(); i++) {
            if (textures[i] == texture) {
                textureIndex = (float)i;
                break;
            }
        }

        // if not, add it
        if (textureIndex == -1.0f) {
            if (textures.size() >= MAX_TEXTURE_SLOTS) {
                flush();
            }
            textureIndex = (float)textures.size();
            textures.emplace_back(texture);
        }
    }

    // quad vertices
    glm::vec3 positions[4] = {
        { position.x, position.y, 0.0f },                       // upper left
        { position.x + size.x, position.y, 0.0f },              // upper right
        { position.x + size.x, position.y + size.y, 0.0f },     // bottom right
        { position.x, position.y + size.y, 0.0f }               // bottom left
    };

    glm::vec2 texCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    // add vertices to the batch
    for (int i = 0; i < 4; i++) {
        vertices[vertexCount].position = positions[i];
        vertices[vertexCount].color = color;
        vertices[vertexCount].texCoord = texCoords[i];
        vertices[vertexCount].texIndex = textureIndex;
        vertexCount++;
    }

    indexCount += 6;
}

void Renderer::drawEntity(ecs::EntityId entity) {
    if (!world->hasComponent<Position>(entity)) {
        return;
    }

    Position* pos = world->getComponent<Position>(entity);
    Scale* scale = world->getComponent<Scale>(entity);

    float scaleX = 1.0f, scaleY = 1.0f;

    if (scale) {
        scaleX = scale->x;
        scaleY = scale->y;
    }

    if (world->hasComponent<Sprite>(entity)) {
        Sprite* sprite = world->getComponent<Sprite>(entity);
        drawQuad(
            glm::vec2(pos->x, pos->y),
            glm::vec2(
                sprite->texture->width * scaleX,
                sprite->texture->height * scaleY
            ),
            glm::vec4(1.0f),
            sprite->texture
        );
    }
    if (world->hasComponent<Quad>(entity)) {
        Quad* quad = world->getComponent<Quad>(entity);
        drawQuad(
            glm::vec2(pos->x, pos->y),
            glm::vec2(
                quad->width * scaleX,
                quad->height * scaleY
            ),
            glm::vec4(1.0f),
            nullptr
        );
    }
}

} // graphics
} // nebula