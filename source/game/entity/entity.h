#ifndef ENTITY_H
#define ENTITY_H

#include "core/minimal.h"

class Entity
{
public:
    Entity(vector2f position, vector2f bounds, float restitution);
    virtual ~Entity() = default;

    virtual void Update(float delta);
    virtual void Render() const = 0;

    void SetPosition(vector2f position);
    void SetVelocity(vector2f velocity);

    vector2f GetPosition() const;
    vector2f GetBounds() const;
    vector2f GetVelocity() const;

protected:
    vector2f position;
    vector2f bounds;
    vector2f velocity;
    float restitution;
};

#endif