#include "save_slot.h"
#include "game/level/level_list.h"
#include <filesystem>
#include <fstream>

SaveSlot* pSave;

// Load the game from a slot.

SaveSlot::SaveSlot(std::string_view path)
    : path(path)
{
    pSave = this;

    std::ifstream file(path.data(), std::ios::binary);

    // If the file was not opened, use a default save.

    if (!file.is_open())
    {
        for (int i = 0; i < levelCount; i++)
        {
            levels[levelList[i].data()] = {i == 0, false, 0.0};
        }

        LOG("No save slot found, one will be generated upon exit.");

        return;
    }

    // Read game data from a file.

    int count;
    file.read((char*) &count, 4);

    for (int i = 0; i < count; i++)
    {
        int nameLength;
        file.read((char*) &nameLength, 4);

        char* name = new char[nameLength + 1];
        name[nameLength] = '\0';
        bool unlocked;
        bool completed;
        double time;

        file.read(name, nameLength);
        file.read((char*) &unlocked, 1);
        file.read((char*) &completed, 1);
        file.read((char*) &time, 8);

        levels[name] = {unlocked, completed, time};

        delete[] name;
    }

    LOG("Loaded save slot from \"" << path << "\".");
}

// Save the game to a slot.

SaveSlot::~SaveSlot()
{
    // Generate a directory for the saves.

    std::filesystem::path directory(path);
    std::filesystem::create_directories(directory.remove_filename());

    // Write game data to a file.

    std::ofstream file(path.data(), std::ios::binary);

    int count = (int) levels.size();
    file.write((char*) &count, 4);

    for (auto[name, data] : levels)
    {
        int nameLength = (int) name.length();
        file.write((char*) &nameLength, 4);

        file.write((char*) name.c_str(), nameLength);
        file.write((char*) &data.unlocked, 1);
        file.write((char*) &data.completed, 1);
        file.write((char*) &data.time, 8);
    }

    LOG("Saved save slot to \"" << path << "\".");
}

// Unlock a level.

void SaveSlot::UnlockLevel(std::string_view name)
{
    if (levels.find(name.data()) != levels.end())
    {
        levels[name.data()].unlocked = true;
    }
    else
    {
        levels[name.data()] = {true, false, 0.0};
    }

    LOG("Wrote level \"" << name << "\" unlock to save.");
}

// Complete a level with time.

void SaveSlot::CompleteLevel(std::string_view name, double time)
{
    if (levels.find(name.data()) != levels.end())
    {
        LevelData& data = levels[name.data()];

        // Only save completion if this is a new record.

        if (!data.completed || time < data.time)
        {
            data.time = time;
            data.completed = true;
        }
    }
    else
    {
        levels[name.data()] = {true, true, time};
    }

    LOG("Wrote level \"" << name << "\" completion to save.");
}

// Check if a level has been unlocked.

bool SaveSlot::IsLevelUnlocked(std::string_view name) const
{
    if (levels.find(name.data()) != levels.end())
    {
        return levels.at(name.data()).unlocked;
    }

    return true;
}

// Check if a level has been completed.

bool SaveSlot::IsLevelCompleted(std::string_view name) const
{
    if (levels.find(name.data()) != levels.end())
    {
        return levels.at(name.data()).completed;
    }

    return false;
}

// Get the completion time of the level.

double SaveSlot::GetLevelTime(std::string_view name) const
{
    if (levels.find(name.data()) != levels.end())
    {
        return levels.at(name.data()).time;
    }

    return 0.0;
}