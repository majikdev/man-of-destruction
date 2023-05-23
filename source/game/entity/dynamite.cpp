#include "dynamite.h"
#include "core/video/renderer.h"
#include "game/level/level.h"

// Initialise the entity.

Dynamite::Dynamite(vector2f position)
    : Entity(position, vector2f(0.375f, 0.375f), 0.5f),
      fuseTime(0.0f), dynamiteSprites()
{
    SpriteSheet sheet = pRenderer->GetSheet("assets/sprites/entity/dynamite.bmp");

    for (int i = 0; i < 3; i++)
    {
        int x = (i % 2) * 4;
        int y = (i / 2) * 4;

        dynamiteSprites[i] = sheet.GetSprite(x, y, 4, 4);
    }
}

// Update the entity.

void Dynamite::Update(float delta)
{
    Entity::Update(delta);

    velocity = Lerp(velocity, vector2f::zero, delta * 4.0f);

    // Explode after a second-long fuse.

    fuseTime += delta;

    if (fuseTime >= 1.0f)
    {
        pLevel->Explode(position);
        pLevel->Destroy(this);
    }
}

// Render the entity.

void Dynamite::Render() const
{
    int index = (int) (fuseTime * 3.0f);
    const Sprite& sprite = dynamiteSprites[index];

    pRenderer->DrawSprite(sprite, position.x - 0.1875f, position.y, 0.3f, 0.5f, 0.5f);
}