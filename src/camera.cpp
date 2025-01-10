#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Camera::update(float dt)
{
    glm::mat4 cameraRotation = getRotationMatrix();
    position += glm::vec3(cameraRotation * glm::vec4(velocity * speed * dt, 0.f));
}

void Camera::processSDLEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_w) { velocity.z = -1; }
        if (e.key.keysym.sym == SDLK_s) { velocity.z = 1; }
        if (e.key.keysym.sym == SDLK_a) { velocity.x = -1; }
        if (e.key.keysym.sym == SDLK_d) { velocity.x = 1; }
        if (e.key.keysym.sym == SDLK_e) { velocity.y = 1; }
        if (e.key.keysym.sym == SDLK_q) { velocity.y = -1; }
    }

    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_w) { velocity.z = 0; }
        if (e.key.keysym.sym == SDLK_s) { velocity.z = 0; }
        if (e.key.keysym.sym == SDLK_a) { velocity.x = 0; }
        if (e.key.keysym.sym == SDLK_d) { velocity.x = 0; }
        if (e.key.keysym.sym == SDLK_e) { velocity.y = 0; }
        if (e.key.keysym.sym == SDLK_q) { velocity.y = 0; }
    }

    // Track right mouse button press/release
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
        rightMouseButtonDown = true;
        SDL_SetRelativeMouseMode(SDL_TRUE); // Enable relative mouse mode
    }
    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT) {
        rightMouseButtonDown = false;
        SDL_SetRelativeMouseMode(SDL_FALSE); // Disable relative mouse mode
    }

    if (e.type == SDL_MOUSEMOTION && rightMouseButtonDown) {
        yaw += (float)e.motion.xrel / 200.f;
        pitch -= (float)e.motion.yrel / 200.f;
    }

    // Mouse wheel for zoom or speed control
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) {  // Wheel scrolled up
            speed += 1.0f;  // Increase camera speed
        }
        else if (e.wheel.y < 0) {  // Wheel scrolled down
            speed -= 1.0f;  // Decrease camera speed
        }

        // Clamp speed to a reasonable range
        speed = glm::clamp(speed, 10.0f, 50.0f);
    }
}

glm::mat4 Camera::getViewMatrix()
{
    // to create a correct model view, we need to move the world in opposite
    // direction to the camera
    //  so we will create the camera model matrix and invert
    glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.f), position);
    glm::mat4 cameraRotation = getRotationMatrix();
    return glm::inverse(cameraTranslation * cameraRotation);
}

glm::mat4 Camera::getRotationMatrix()
{
    // fairly typical FPS style camera. we join the pitch and yaw rotations into
    // the final rotation matrix

    glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3{ 1.f, 0.f, 0.f });
    glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3{ 0.f, -1.f, 0.f });

    return glm::toMat4(yawRotation) * glm::toMat4(pitchRotation);
}