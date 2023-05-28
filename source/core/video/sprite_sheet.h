#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "sprite.h"

struct SpriteSheet
{
public:
    Sprite GetSprite(int x, int y, int w, int h) const
    {
        y = imageHeight - y - h;

        return {identifier, (float) x / (float) imageWidth, (float) y / (float) imageHeight,
                            (float) w / (float) imageWidth, (float) h / (float) imageHeight};
    }

public:
    unsigned int identifier;
    int imageWidth;
    int imageHeight;
};

#endif