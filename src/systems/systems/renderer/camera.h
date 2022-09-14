#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace Renderer {
enum class CameraControl {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FOV_INC,
    FOV_DEC,
    SPEED_INC,
    SPEED_DEC,
    RESET
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
   public:
    // camera Attributes
    const glm::vec3 worldUp{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // euler Angles without roll
    float yaw;
    float pitch;

    // camera options
    float movementSpeed = SPEED;
    float mouseSensitivity = SENSITIVITY;
    float fov = FOV;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH) : position{position},
                                  yaw{yaw},
                                  pitch{pitch} {
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraControl direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    void Reset() {
        fov = FOV;
        movementSpeed = SPEED;
        mouseSensitivity = SENSITIVITY;

        yaw = YAW;
        pitch = PITCH;

        position.x = {0};
        front = {0, 0, -1};
    }

   private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
}  // namespace Renderer