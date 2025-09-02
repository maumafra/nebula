#include "Camera.h"

namespace nebula {
    namespace graphics {

Camera::Camera(int width,
    int height,
    float nearClip,
    float farClip)
    : width((float)width)
    , height((float)height)
    , nearClip(nearClip)
    , farClip(farClip) 
    , projectionMatrix(glm::ortho(0.0f, (float)width, (float)height, 0.0f, nearClip, farClip))
    , viewMatrix(glm::mat4(1.0f))
    {}

void Camera::moveCamera(glm::vec2 move) {
    this->position.x += move.x;
    this->position.y += move.y;
    recalculateView();
}

void Camera::set(int width, int height, float nearClip, float farClip) {
    this->width = (float)width;
    this->height = (float)height;
    this->nearClip = nearClip;
    this->farClip = farClip;
    recalculateProjection();
}

void Camera::setSize(int width, int height) {
    this->width = (float)width;
    this->height = (float)height;
    recalculateProjection();
}

void Camera::setClip(float nearClip, float farClip) {
    this->nearClip = nearClip;
    this->farClip = farClip;
    recalculateProjection();
}

float Camera::getNearClip() {
    return this->nearClip;
}

float Camera::getFarClip() {
    return this->farClip;
}

glm::vec2 Camera::getPosition() {
    return this->position;
}

glm::mat4 Camera::getProjection() {
    return this->projectionMatrix;
}

glm::mat4 Camera::getView() {
    return this->viewMatrix;
}

void Camera::recalculateView() {
    this->viewMatrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            this->position.x,
            this->position.y,
            0.0f
        )
    );
}

void Camera::recalculateProjection() {
    this->projectionMatrix = glm::ortho(0.0f, width, height, 0.0f, nearClip, farClip);
}

} // graphics
} // nebula