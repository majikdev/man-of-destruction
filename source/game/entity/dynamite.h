#ifndef DYNAMITE_H
#define DYNAMITE_H

#include "entity.h"

class Dynamite : public Entity
{
public:
    Dynamite(vector2f position);

    void Update(float delta) override;
    void Render() const override;

private:
    float fuseTime;

    Sprite dynamiteSprites[3];
};

#endif