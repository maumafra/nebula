#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nebula {
    namespace graphics {

// Maybe make the camera an entity and convert all it's relevant data as a single Camera Component
class Camera {
public:
    Camera(int width, int height, float nearClip = -1.0f, float farClip = 1.0f);
    ~Camera() = default;

    void moveCamera(glm::vec2 move);

    void set(int width, int height, float nearClip = -1.0f, float farClip = 1.0f);
    void setSize(int width, int height);
    void setClip(float nearClip, float farClip);
    
    float getNearClip();
    float getFarClip();

    glm::vec2 getPosition();

    glm::mat4 getProjection();
    glm::mat4 getView();

private:
    void recalculateView();
    void recalculateProjection();

    glm::vec2 position = {0.0f, 0.0f};
    float nearClip = -1.0f, farClip = 1.0f;
    float width, height;

    // TODO: create a zoom in/out impl. check this: https://stackoverflow.com/questions/73422091/what-is-the-proper-way-to-zoom-in-and-out-using-orthographic-projection
    //float zoom = 1.0f;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

} // graphics
} // nebula