#ifndef RENDERER_H
#define RENDERER_H

#include "sprite_sheet.h"
#include <string_view>
#include <unordered_map>

extern class Renderer* pRenderer;

class Renderer
{
public:
    Renderer(std::string_view vertexPath, std::string_view fragmentPath);
    ~Renderer();

    void SetFontSheet(const SpriteSheet& sheet);
    void SetResolution(int width, int height) const;
    void SetProjection(float l, float r, float b, float t, float depth) const;

    void DrawSprite(const Sprite& sprite, float x, float y, float z, float w = 1.0f, float h = 1.0f) const;
    void DrawString(std::string_view string, float x, float y, float z, float alignment = 0.5f) const;
    void Clear() const;

    SpriteSheet GetSheet(std::string_view path);

private:
    unsigned int shaderProgram;
    unsigned int vertexArray;
    unsigned int vertexBuffer;
    unsigned int indexBuffer;

    int projectionUniform;
    int positionUniform;
    int scaleUniform;
    int coordsUniform;

    std::vector<unsigned int> textures;
    std::unordered_map<std::string_view, SpriteSheet> sheets;

    Sprite fontSprites[95];
};

#endif