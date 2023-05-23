#ifndef SPLINTER_H
#define SPLINTER_H

#include "entity.h"

class Player;

class Splinter : public Entity
{
public:
    Splinter(vector2f position);

    void Update(float delta) override;
    void Render() const override;

private:
    Sprite sprite;

    Player* pPlayer;
};

#endif