#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
public:
    Player(vector2f position);

    void Update(float delta) override;
    void Render() const override;

    void Damage(int amount);
    bool GiveDynamite();

    bool IsAlive() const;

private:
    int health;
    int dynamite;
    float invincibleTime;
    float animationTime;
    int stepCount;

    Sprite playerSprites[18];
    Sprite hudSprites[5];
    Sound stepSounds[5];
    Sound hurtSound;
    Sound throwSound;
};

#endif