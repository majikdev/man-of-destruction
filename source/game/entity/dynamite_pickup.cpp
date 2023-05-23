#include "dynamite_pickup.h"
#include "core/audio/sound_mixer.h"
#include "core/video/renderer.h"
#include "game/entity/player.h"
#include "game/level/level.h"

// Initialise the entity.

DynamitePickup::DynamitePickup(vector2f position)
    : Entity(position, vector2f(0.375f, 0.375f), 0.5f),
      bobbingTime(0.0f), pickupSprite(), pickupSound()
{
    SpriteSheet sheet = pRenderer->GetSheet("assets/sprites/entity/dynamite_pickup.bmp");
    pickupSprite = sheet.GetSprite(0, 0, 4, 6);

    pickupSound = pSoundMixer->GetSound("assets/sounds/dynamite_pickup.wav");

    // Save the player to check distance in Update.

    pPlayer = pLevel->GetEntity<Player>();
}

// Update the entity.

void DynamitePickup::Update(float delta)
{
    Entity::Update(delta);

    velocity = Lerp(velocity, vector2f::zero, delta * 4.0f);

    // Timer for bobbing up and down.

    bobbingTime += delta;

    if (bobbingTime >= pi)
    {
        bobbingTime -= pi;
    }

    // Give the player a dynamite when close.

    if ((position - pPlayer->GetPosition()).SqrLength() < 0.5f)
    {
        if (pPlayer->GiveDynamite())
        {
            pSoundMixer->PlaySound(pickupSound);
            pLevel->Destroy(this);
        }
    }
}

// Render the entity.

void DynamitePickup::Render() const
{
    float height = (sin(bobbingTime * 4.0f) + 1.0f) * 0.125f;

    pRenderer->DrawSprite(pickupSprite, position.x - 0.25f, position.y + height, 0.4f, 0.5f, 0.75f);
}