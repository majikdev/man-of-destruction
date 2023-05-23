#ifndef CAMERA_H
#define CAMERA_H

#include "core/minimal.h"

extern class Camera* pCamera;

class Camera
{
public:
    Camera();

    void Update(float delta);
    void ApplyWorldProjection() const;
    void ApplyScreenProjection() const;

    void SetPosition(vector2f position);
    void SetUnitScale(int scale);
    void SetShakeStrength(float strength);
    void ApplyCameraShake(float strength);

    vector2f GetPosition() const;
    vector2f GetBounds() const;

private:
    vector2f position;
    vector2f bounds;
    vector2f shake;
    float shakeTime;

    float unitScale;
    float shakeAmount;
};

#endif