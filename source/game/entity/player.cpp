#include "player.h"
#include "core/audio/sound_mixer.h"
#include "core/input/controller.h"
#include "core/video/renderer.h"
#include "game/camera/camera.h"
#include "game/entity/dynamite.h"
#include "game/level/level.h"
#include "game/menu/pause_menu.h"

// Initialise the entity.

Player::Player(vector2f position)
    : Entity(position, vector2f(0.375f, 0.5f), 0.0f),
      health(3), dynamite(3), stepCount(0), invincibleTime(0.0f), animationTime(0.0f),
      playerSprites(), hudSprites(), stepSounds(), hurtSound(), throwSound()
{
    SpriteSheet playerSheet = pRenderer->GetSheet("assets/sprites/entity/player.bmp");
    SpriteSheet uiSheet = pRenderer->GetSheet("assets/sprites/widget/widget.bmp");

    for (int i = 0; i < 18; i++)
    {
        int x = (i % 8) * 5;
        int y = (i / 8) * 7;

        playerSprites[i] = playerSheet.GetSprite(x, y, 5, 7);
    }

    for (int i = 0; i < 5; i++)
    {
        hudSprites[i] = uiSheet.GetSprite(i * 8, 48, 8, 8);
    }

    stepSounds[0] = pSoundMixer->GetSound("assets/sounds/player_step_a.wav");
    stepSounds[1] = pSoundMixer->GetSound("assets/sounds/player_step_b.wav");
    stepSounds[2] = pSoundMixer->GetSound("assets/sounds/player_step_c.wav");
    stepSounds[3] = pSoundMixer->GetSound("assets/sounds/player_step_d.wav");
    stepSounds[4] = pSoundMixer->GetSound("assets/sounds/player_step_e.wav");

    hurtSound = pSoundMixer->GetSound("assets/sounds/player_hurt.wav");
    throwSound = pSoundMixer->GetSound("assets/sounds/dynamite_throw.wav");
}

// Update the entity.

void Player::Update(float delta)
{
    Entity::Update(delta);

    // Make the camera follow the player.

    pCamera->SetPosition(Lerp(pCamera->GetPosition(), GetPosition(), delta * 4.0f));

    // Pause the game with Escape.

    if (pController->WasPressed(KEY_ESCAPE))
    {
        Menu::Open<PauseMenu>();
    }

    // Retry the level with F5.

    if (pController->WasPressed(KEY_F5))
    {
        Level::Load(pLevel->GetName());
    }

    if (IsAlive())
    {
        // Momentum-based movement with W, A, S, D.

        vector2f input;
        input.x = (float) (pController->IsHeldDown(KEY_D) - pController->IsHeldDown(KEY_A));
        input.y = (float) (pController->IsHeldDown(KEY_W) - pController->IsHeldDown(KEY_S));

        velocity = Lerp(velocity, input.Normalised() * 3.0f, delta * 16.0f);

        // Throw a stick of dynamite towards the mouse cursor with Left Mouse Button.

        if (pController->WasPressed(MOUSE_LEFT) && dynamite > 0)
        {
            vector2f targetPosition = pController->GetMousePosition() + pCamera->GetPosition();
            vector2f direction = (targetPosition - position).Normalised();

            Dynamite* pDynamite = pLevel->Instantiate<Dynamite>(position);
            pDynamite->SetVelocity(direction * 4.0f);

            dynamite--;

            pSoundMixer->PlaySound(throwSound);
        }

        // Check if the finish is reached.

        vector2f finish = pLevel->GetFinish();

        if ((int) position.x == (int) finish.x && (int) position.y == (int) finish.y)
        {
            pLevel->Complete();
        }

        // Decrement the invincibility timer.

        if (invincibleTime > 0.0f)
        {
            invincibleTime -= delta;
        }

        // Timer for the walk animation and footsteps.

        animationTime += delta;

        if (animationTime >= 0.333f)
        {
            animationTime -= 0.333f;

            if (velocity.Length() > 0.01f)
            {
                pSoundMixer->PlaySound(stepSounds[stepCount]);
                stepCount = (stepCount + 1) % 5;
            }
        }
    }
    else
    {
        velocity = Lerp(velocity, vector2f::zero, delta * 8.0f);
    }
}

// Render the entity.

void Player::Render() const
{
    int index = IsAlive() ? 16 : 17;

    // If the player is moving, choose a walking sprite;
    // Otherwise an idle alive/dead sprite is used.

    if (IsAlive() && velocity.SqrLength() > 0.01f)
    {
        int direction = (int) ((velocity.Rotation() + 22.5f) / 45.0f) % 8;
        index = (int) (animationTime * 6.0f) * 8 + direction;
    }

    pRenderer->DrawSprite(playerSprites[index], position.x - 0.3125f, position.y, 0.5f, 0.625f, 0.875f);

    // Render the heads-up display.

    pCamera->ApplyScreenProjection();
    float hudHeight = pCamera->GetBounds().y;

    pRenderer->DrawSprite(hudSprites[0], -2.0f, hudHeight - 0.75f, 3.0f, 0.5f, 0.5f);
    pRenderer->DrawString(Level::TimeToString(pLevel->GetTime()), -1.375f, hudHeight - 0.25f, 3.0f, 0.0f);

    for (int i = 0; i < 3; i++)
    {
        index = (i < health) ? 1 : 2;

        float x = -1.625f + (float) i * 0.5f;
        float y = hudHeight - 1.375f;

        pRenderer->DrawSprite(hudSprites[index], x, y, 3.0f, 0.5f, 0.5f);
    }

    for (int i = 0; i < 3; i++)
    {
        index = (i < dynamite) ? 3 : 4;

        float x = 0.125f + (float) i * 0.5f;
        float y = hudHeight - 1.375f;

        pRenderer->DrawSprite(hudSprites[index], x, y, 3.0f, 0.5f, 0.5f);
    }

    // Prompt the player to retry when dead.

    if (!IsAlive())
    {
        pRenderer->DrawString("Press F5 to retry.", 0.0f, hudHeight * -0.6f, 3.0f);
    }

    pCamera->ApplyWorldProjection();
}

// Damage the player.

void Player::Damage(int amount)
{
    // Only damage if the player can be damaged;
    // Then apply invincibility for half a second.

    if (IsAlive() && invincibleTime <= 0.0f)
    {
        health = Max(health - amount, 0);
        invincibleTime = 0.5f;

        pSoundMixer->PlaySound(hurtSound);
    }
}

// Give the player one dynamite;
// Returns true if successful.

bool Player::GiveDynamite()
{
    if (dynamite < 3)
    {
        dynamite++;

        return true;
    }

    return false;
}

// Check if the player is alive.

bool Player::IsAlive() const
{
    return health > 0;
}