#include "level.h"
#include "level_list.h"
#include "core/audio/sound_mixer.h"
#include "core/video/renderer.h"
#include "game/camera/camera.h"
#include "game/entity/player.h"
#include "game/entity/dynamite_pickup.h"
#include "game/entity/splinter.h"
#include "game/menu/level_complete_menu.h"
#include "game/save/save_slot.h"
#include <fstream>

std::shared_ptr<Level> pLevel;

// Load level data from a file.

static std::vector<Tile> LoadLevelFile(std::string_view path, std::string& outBiome, int& outWidth, int& outHeight, vector2f& outStart, vector2f& outFinish, std::vector<vector2f>& outDynamites)
{
    std::ifstream file(path.data(), std::ios::binary);
    std::vector<Tile> tiles;

    // Validate that the file was opened.

    if (!file.is_open())
    {
        ERR("Failed to load level from \"" << path << "\".");

        return std::move(tiles);
    }

    // Read the environment of the level.

    int biomeLength;
    file.read((char*) &biomeLength, 4);

    char* biome = new char[biomeLength + 1];
    biome[biomeLength] = '\0';

    file.read(biome, biomeLength);

    outBiome = biome;
    delete[] biome;

    // Read the dimensions of the level.

    file.read((char*) &outWidth, 4);
    file.read((char*) &outHeight, 4);

    // Read the start and finish positions.

    int xStart, yStart;
    file.read((char*) &xStart, 4);
    file.read((char*) &yStart, 4);

    int xFinish, yFinish;
    file.read((char*) &xFinish, 4);
    file.read((char*) &yFinish, 4);

    outStart = vector2f((float) xStart + 0.5f, (float) yStart + 0.5f);
    outFinish = vector2f((float) xFinish + 0.5f, (float) yFinish + 0.5f);

    // Read all dynamite pick-up positions.

    int dynamiteCount;
    file.read((char*) &dynamiteCount, 4);

    outDynamites.reserve(dynamiteCount);

    for (int i = 0; i < dynamiteCount; i++)
    {
        int xDynamite, yDynamite;
        file.read((char*) &xDynamite, 4);
        file.read((char*) &yDynamite, 4);

        outDynamites.emplace_back((float) xDynamite + 0.5f, (float) yDynamite + 0.5f);
    }

    // Read the level's tile data.

    tiles.reserve(outWidth * outHeight);

    for (int i = 0; i < outWidth * outHeight; i++)
    {
        char tile;
        file.read((char*) &tile, 1);

        tiles.push_back({(int) tile, 0});
    }

    // Set the tile's variants.

    for (int i = 0; i < outWidth * outHeight; i++)
    {
        Tile& tile = tiles[i];
        int x = i % outWidth;
        int y = i / outWidth;

        // Ground varies based on the tile above it.

        if (tile.type == 0)
        {
            tile.variant = tiles[i + outWidth].type;
        }

        // Walls vary based on their neighbours.

        else if (tile.type == 4)
        {
            bool offBottom = (y > 0);
            bool offTop = (y < outHeight - 1);
            bool offLeft = (x > 0);
            bool offRight = (x < outWidth - 1);

            bool bottom = offBottom && tiles[i - outWidth].type != 4;
            bool top = offTop && tiles[i + outWidth].type != 4;
            bool left = offLeft && tiles[i - 1].type != 4;
            bool right = offRight && tiles[i + 1].type != 4;

            bool bottomLeft = offBottom && offLeft && tiles[i - outWidth - 1].type != 4;
            bool bottomRight = offBottom && offRight && tiles[i - outWidth + 1].type != 4;
            bool topLeft = offTop && offLeft && tiles[i + outWidth - 1].type != 4;
            bool topRight = offTop && offRight && tiles[i + outWidth + 1].type != 4;

            tile.variant = bottom << 7 | top << 6 | left << 5 | right << 4 | bottomLeft << 3 | bottomRight << 2 | topLeft << 1 | topRight;
        }
    }

    LOG("Loaded level from \"" << path << "\".");

    return std::move(tiles);
}

// Initialise the level.

Level::Level(std::string name)
    : name(std::move(name)), tileTypes(), playTime(0.0), levelWidth(0),
      levelHeight(0), sprites(), explodeSound(), completeSound()
{
    pLevel.reset(this);

    // Register the tile types.

    using namespace std::placeholders;

    tileTypes[0] = {false, false, 0,  nullptr};                                          // Ground tile.
    tileTypes[1] = {true,  true,  5,  nullptr};                                          // Stone tile.
    tileTypes[2] = {true,  true,  6,  std::bind(&Level::OnWoodBreak, this, _1, _2)};     // Wood tile.
    tileTypes[3] = {true,  true,  7,  std::bind(&Level::OnDynamiteBreak, this, _1, _2)}; // Dynamite tile.
    tileTypes[4] = {true,  false, 10, nullptr};                                          // Wall tile.

    // Load the level from a file.

    std::string path = "levels/" + this->name + ".level";
    std::string biome;
    std::vector<vector2f> dynamitePositions;

    tiles = LoadLevelFile(path, biome, levelWidth, levelHeight, start, finish, dynamitePositions);

    // Load the necessary resources.

    SpriteSheet levelSheet = pRenderer->GetSheet("assets/sprites/level/" + biome + ".bmp");
    SpriteSheet wallSheet = pRenderer->GetSheet("assets/sprites/level/walls.bmp");

    for (int i = 0; i < 10; i++)
    {
        int x = (i % 4) * 8;
        int y = (i / 4) * 8;

        sprites[i] = levelSheet.GetSprite(x, y, 8, 8);
    }

    for (int i = 0; i < 256; i++)
    {
        int x = (i % 16) * 8;
        int y = (i / 16) * 8;

        sprites[i + 10] = wallSheet.GetSprite(x, y, 8, 8);
    }

    explodeSound = pSoundMixer->GetSound("assets/sounds/explosion.wav");
    completeSound = pSoundMixer->GetSound("assets/sounds/level_complete.wav");

    // Spawn the initial entities.

    Instantiate<Player>(start);

    for (const vector2f& position : dynamitePositions)
    {
        Instantiate<DynamitePickup>(position);
    }

    LOG("Instantiated the Level (" << this->name << ").");
}

// Terminate the level.

Level::~Level()
{
    entities.clear();

    LOG("Destroyed the Level.");
}

// Update the level and its entities.

void Level::Update(float delta)
{
    playTime += (double) delta;

    // Update all entities in the level.

    for (int i = 0; i < entities.size(); i++)
    {
        // Stop iterating if level changed.
        if (this != pLevel.get())
        {
            return;
        }

        entities[i]->Update(delta);
    }
}

// Render the level and its entities.

void Level::Render() const
{
    pCamera->ApplyWorldProjection();

    // Draw all tiles in the level.

    for (int i = 0; i < levelWidth * levelHeight; i++)
    {
        const Tile& tile = tiles[i];
        const TileType& type = tileTypes[tile.type];
        float x = (float) (i % levelWidth);
        float y = (float) (i / levelWidth) + 0.75f;

        pRenderer->DrawSprite(sprites[type.spriteOffset + tile.variant], x, y, type.solid ? 1.0f : 0.0f);
    }

    pRenderer->DrawSprite(sprites[8], finish.x - 0.5f, finish.y - 0.5f, 0.1f);
    pRenderer->DrawSprite(sprites[9], finish.x - 0.5f, finish.y + 0.5f, 1.1f);

    // Draw all entities in the level.

    for (int i = 0; i < entities.size(); i++)
    {
        entities[i]->Render();
    }
}

// Destroy an entity.

void Level::Destroy(Entity* pEntity)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i].get() == pEntity)
        {
            entities.erase(entities.begin() + i);

            return;
        }
    }
}

// Create an explosion at a position.

void Level::Explode(vector2f position)
{
    pCamera->ApplyCameraShake(1.0f);
    pSoundMixer->PlaySound(explodeSound);

    // Push away all nearby entities.

    for (int i = 0; i < entities.size(); i++)
    {
        Entity* pEntity = entities[i].get();
        vector2f offset = pEntity->GetPosition() - position;

        if (offset.SqrLength() < 4.0f)
        {
            pEntity->SetVelocity(offset.Normalised() * 4.0f);

            // If a nearby entity is a player, damage it.

            Player* pPlayer = dynamic_cast<Player*>(pEntity);

            if (pPlayer)
            {
                pPlayer->Damage(3);
            }
        }
    }

    // Break all tiles in a 3 by 3 area.

    int x = (int) position.x;
    int y = (int) position.y;

    for (int ty = y - 1; ty <= y + 1; ty++)
    {
        for (int tx = x - 1; tx <= x + 1; tx++)
        {
            if (tx != x || ty != y)
            {
                Break(tx, ty);
            }
        }
    }
}

// Break a tile at a position.

void Level::Break(int x, int y)
{
    int index = y * levelWidth + x;
    Tile& tile = tiles[index];

    if (tileTypes[tile.type].breakable)
    {
        auto onBreak = tileTypes[tile.type].pOnBreak;
        tile = {0, tiles[index + levelWidth].type};

        // Call the break callback if one exists.

        if (onBreak)
        {
            onBreak(x, y);
        }

        // Update the variant of the tile below.

        if (tiles[index - levelWidth].type == 0)
        {
            tiles[index - levelWidth].variant = 0;
        }
    }
}

// Complete the level.

void Level::Complete()
{
    pSoundMixer->PlaySound(completeSound);
    pSave->CompleteLevel(name, playTime);

    // Unlock the next level if this is a base game level.

    int i = levelCount;
    auto location = std::find(std::begin(levelList), std::end(levelList), name);

    if (location != std::end(levelList))
    {
        i = location - std::begin(levelList);

        // Only unlock the next level if one exists.

        if (i < levelCount - 1)
        {
            pSave->UnlockLevel(levelList[i + 1]);
        }
    }

    Menu::Open<LevelCompleteMenu>(i, name);
}

// Get the level's name.

std::string_view Level::GetName() const
{
    return name;
}

// Get the level's start position.

vector2f Level::GetStart() const
{
    return start;
}

// Get the level's finish position.

vector2f Level::GetFinish() const
{
    return finish;
}

// Get the level's play time.

double Level::GetTime() const
{
    return playTime;
}

// Check if a tile is solid.

bool Level::IsSolid(int x, int y) const
{
    return tileTypes[tiles[y * levelWidth + x].type].solid;
}

// Wood tile break callback.

void Level::OnWoodBreak(int x, int y)
{
    vector2f position((float) x + 0.5f, (float) y + 0.5f);

    // Spawn 8 splinters each flying in a different direction.

    for (int i = 0; i < 8; i++)
    {
        float angle = (float) i * 45.0f + 22.5f;

        Splinter* pSplinter = Instantiate<Splinter>(position);
        pSplinter->SetVelocity(vector2f(32.0f, 0.0f).Rotated(angle));
    }
}

// Dynamite tile break callback.

void Level::OnDynamiteBreak(int x, int y)
{
    vector2f position((float) x + 0.5f, (float) y + 0.5f);

    Explode(position);
}

// Load a level from a name.

void Level::Load(std::string_view name)
{
    new Level(std::string(name));

    pCamera->SetPosition(pLevel->GetStart());
}

// Unload the current level.

void Level::Unload()
{
    pLevel.reset();

    pCamera->SetPosition(vector2f::zero);
}

// Convert time to a formatted string.

std::string Level::TimeToString(double time)
{
    std::string string = "00:00.000";

    int minutes = (int) time / 60;
    int seconds = (int) time % 60;
    int milliseconds = (int) (fmod(time, 1.0) * 1000.0);

    string[0] = (char) (minutes / 10 + 48);
    string[1] = (char) (minutes % 10 + 48);

    string[3] = (char) (seconds / 10 + 48);
    string[4] = (char) (seconds % 10 + 48);

    for (int i = 2; i >= 0; i--)
    {
        string[i + 6] = (char) (milliseconds % 10 + 48);
        milliseconds /= 10;
    }

    return std::move(string);
}

// Convert a level file name to a formatted name.

std::string Level::FormatName(std::string name)
{
    for (int i = 0; i < name.length(); i++)
    {
        // Replace underscores with spaces.

        if (name[i] == '_')
        {
            name[i] = ' ';
        }

        // Capitalise first letters of words.

        else if (i == 0 || name[i - 1] == ' ')
        {
            name[i] = (char) std::toupper(name[i]);
        }
    }

    return std::move(name);
}