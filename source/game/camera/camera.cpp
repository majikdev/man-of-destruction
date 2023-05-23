#include "camera.h"
#include "core/video/renderer.h"
#include "core/video/window.h"

Camera* pCamera;

// Initialise the camera.

Camera::Camera()
    : shakeTime(0.0f), unitScale(64.0f), shakeAmount(1.0f)
{
    pCamera = this;
}

// Update the camera.

void Camera::Update(float delta)
{
    bounds.x = (float) pWindow->GetWidth() / (unitScale * 2.0f);
    bounds.y = (float) pWindow->GetHeight() / (unitScale * 2.0f);

    // Shake the camera.

    constexpr float shakeInterval = 0.033f;

    if (!shake.IsNearlyZero())
    {
        shakeTime += delta;

        if (shakeTime >= shakeInterval)
        {
            shakeTime = 0.0f;
            shake *= vector2f(-0.5f, -0.75f);
        }
    }
}

// Apply a projection for viewing the level.

void Camera::ApplyWorldProjection() const
{
    float x = Snap(position.x + shake.x, 1.0f / unitScale);
    float y = Snap(position.y + shake.y, 1.0f / unitScale);

    pRenderer->SetProjection(x - bounds.x, x + bounds.x, y - bounds.y, y + bounds.y, 4.0f);
}

// Apply a projection for viewing the menus.

void Camera::ApplyScreenProjection() const
{
    pRenderer->SetProjection(-bounds.x, bounds.x, -bounds.y, bounds.y, 4.0f);
}

// Set the camera's position.

void Camera::SetPosition(vector2f position)
{
    this->position = position;
}

// Set the camera's unit scale.

void Camera::SetUnitScale(int scale)
{
    unitScale = (float) scale;
}

// Set the camera's shake strength.

void Camera::SetShakeStrength(float strength)
{
    shakeAmount = strength;
}

// Apply a shake to the camera.

void Camera::ApplyCameraShake(float strength)
{
    shake = vector2f(1.0f, -0.5f) * (strength * shakeAmount);
}

// Get the camera's position (including shaking).

vector2f Camera::GetPosition() const
{
    return position;
}

// Get the extent of the camera's bounds.

vector2f Camera::GetBounds() const
{
    return bounds;
}