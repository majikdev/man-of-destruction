#include "splinter.h"
#include "core/video/renderer.h"
#include "game/entity/player.h"
#include "game/level/level.h"

// Initialise the entity.

Splinter::Splinter(vector2f position)
    : Entity(position, vector2f(0.375f, 0.375f), 1.0f),
      sprite()
{
    SpriteSheet sheet = pRenderer->GetSheet("assets/sprites/entity/splinter.bmp");
    sprite = sheet.GetSprite(0, 0, 3, 3);

    // Save the player to check distance in Update.

    pPlayer = pLevel->GetEntity<Player>();
}

// Update the entity.

void Splinter::Update(float delta)
{
    Entity::Update(delta);

    velocity = Lerp(velocity, vector2f::zero, delta * 4.0f);

    // Damage the player when close and moving quickly.

    if (velocity.SqrLength() > 4.0f && (position - pPlayer->GetPosition()).SqrLength() < 0.5f)
    {
        pPlayer->Damage(1);
    }
}

// Render the entity.

void Splinter::Render() const
{
    pRenderer->DrawSprite(sprite, position.x - 0.1875f, position.y - 0.1875f, 0.2f, 0.375f, 0.375f);
}