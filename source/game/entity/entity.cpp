#include "entity.h"
#include "game/level/level.h"

// Initialise the entity.

Entity::Entity(vector2f position, vector2f bounds, float restitution)
    : position(position), bounds(bounds), restitution(restitution)
{}

// Update the entity.

void Entity::Update(float delta)
{
    if (!velocity.IsNearlyZero())
    {
        float left = position.x - bounds.x * 0.5f;
        float right = position.x + bounds.x * 0.5f;
        float bottom = position.y - bounds.y * 0.5f;
        float top = position.y + bounds.y * 0.5f;
        float halfWidth = bounds.x * 0.5f + epsilon;
        float halfHeight = bounds.y * 0.5f + epsilon;

        // Prevent collisions when moving right.

        if (velocity.x > 0)
        {
            float xMoved = right + velocity.x * delta;

            if (pLevel->IsSolid((int) xMoved, (int) bottom) || pLevel->IsSolid((int) xMoved, (int) top))
            {
                position.x = ceil(position.x) - halfWidth;
                velocity.x *= -restitution;
            }
            else
            {
                position.x += velocity.x * delta;
            }
        }

        // Prevent collisions when moving left.

        else if (velocity.x < 0)
        {
            float xMoved = left + velocity.x * delta;

            if (pLevel->IsSolid((int) xMoved, (int) bottom) || pLevel->IsSolid((int) xMoved, (int) top))
            {
                position.x = floor(position.x) + halfWidth;
                velocity.x *= -restitution;
            }
            else
            {
                position.x += velocity.x * delta;
            }
        }

        // Prevent collisions when moving up.

        if (velocity.y > 0)
        {
            float yMoved = top + velocity.y * delta;

            if (pLevel->IsSolid((int) left, (int) yMoved) || pLevel->IsSolid((int) right, (int) yMoved))
            {
                position.y = ceil(position.y) - halfHeight;
                velocity.y *= -restitution;
            }
            else
            {
                position.y += velocity.y * delta;
            }
        }

        // Prevent collisions when moving down.

        else if (velocity.y < 0)
        {
            float yMoved = bottom + velocity.y * delta;

            if (pLevel->IsSolid((int) left, (int) yMoved) || pLevel->IsSolid((int) right, (int) yMoved))
            {
                position.y = floor(position.y) + halfHeight;
                velocity.y *= -restitution;
            }
            else
            {
                position.y += velocity.y * delta;
            }
        }
    }
}

// Set the entity's position.

void Entity::SetPosition(vector2f position)
{
    this->position = position;
}

// Set the entity's velocity.

void Entity::SetVelocity(vector2f velocity)
{
    this->velocity = velocity;
}

// Get the entity's position.

vector2f Entity::GetPosition() const
{
    return position;
}

// Get the extent of the entity's bounds.

vector2f Entity::GetBounds() const
{
    return bounds;
}

// Get the entity's velocity.

vector2f Entity::GetVelocity() const
{
    return velocity;
}