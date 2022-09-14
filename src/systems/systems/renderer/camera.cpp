#include "camera.h"

using namespace Renderer;

void Camera::ProcessKeyboard(CameraControl input, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (input == CameraControl::FORWARD)
        position += front * velocity;
    if (input == CameraControl::BACKWARD)
        position -= front * velocity;
    if (input == CameraControl::LEFT)
        position -= right * velocity;
    if (input == CameraControl::RIGHT)
        position += right * velocity;
    if (input == CameraControl::UP)
        position += worldUp * velocity;
    if (input == CameraControl::DOWN)
        position -= worldUp * velocity;
    if (input == CameraControl::RESET) this->Reset();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(this->front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, this->front));
}