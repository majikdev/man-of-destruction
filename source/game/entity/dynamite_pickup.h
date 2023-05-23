#ifndef DYNAMITE_PICKUP_H
#define DYNAMITE_PICKUP_H

#include "entity.h"

class Player;

class DynamitePickup : public Entity
{
public:
    DynamitePickup(vector2f position);

    void Update(float delta) override;
    void Render() const override;

private:
    float bobbingTime;

    Sprite pickupSprite;
    Sound pickupSound;

    Player* pPlayer;
};

#endif