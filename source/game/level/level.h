#ifndef LEVEL_H
#define LEVEL_H

#include "core/minimal.h"
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

class Level;
class Entity;

struct TileType
{
    bool solid;
    bool breakable;
    int spriteOffset;

    std::function<void(int, int)> pOnBreak;
};

struct Tile
{
    int type;
    int variant;
};

extern std::shared_ptr<class Level> pLevel;

class Level
{
public:
    Level(std::string name);
    ~Level();

    void Update(float delta);
    void Render() const;

    template<class T>
    T* Instantiate(vector2f position);
    void Destroy(Entity* pEntity);
    void Explode(vector2f position);
    void Break(int x, int y);
    void Complete();

    template<class T>
    T* GetEntity() const;
    std::string_view GetName() const;
    vector2f GetStart() const;
    vector2f GetFinish() const;
    double GetTime() const;
    bool IsSolid(int x, int y) const;

private:
    void OnWoodBreak(int x, int y);
    void OnDynamiteBreak(int x, int y);

public:
    static void Load(std::string_view name);
    static void Unload();
    static std::string TimeToString(double time);
    static std::string FormatName(std::string name);

private:
    std::string name;

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Tile> tiles;
    TileType tileTypes[5];
    double playTime;

    int levelWidth;
    int levelHeight;
    vector2f start;
    vector2f finish;

    Sprite sprites[266];
    Sound explodeSound;
    Sound completeSound;
};

// Instantiate an entity.

template<class T>
T* Level::Instantiate(vector2f position)
{
    static_assert(std::is_base_of<Entity, T>::value);

    // Create an entity of class and return it.

    entities.push_back(std::make_unique<T>(position));

    return (T*) entities.back().get();
}

// Get the first matching entity.

template<class T>
T* Level::GetEntity() const
{
    static_assert(std::is_base_of<Entity, T>::value);

    // Search all entities, return the first entity of class.

    for (const std::unique_ptr<Entity>& pEntity : entities)
    {
        T* pWanted = dynamic_cast<T*>(pEntity.get());

        if (pWanted)
        {
            return pWanted;
        }
    }

    // There are no entities of such class.

    return nullptr;
}

#endif