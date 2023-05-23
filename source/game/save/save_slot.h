#ifndef SAVE_SLOT_H
#define SAVE_SLOT_H

#include "core/minimal.h"
#include <string_view>
#include <string>
#include <unordered_map>

struct LevelData
{
    bool unlocked;
    bool completed;
    double time;
};

extern class SaveSlot* pSave;

class SaveSlot
{
public:
    SaveSlot(std::string_view path);
    ~SaveSlot();

    void UnlockLevel(std::string_view name);
    void CompleteLevel(std::string_view name, double time);

    bool IsLevelUnlocked(std::string_view name) const;
    bool IsLevelCompleted(std::string_view name) const;
    double GetLevelTime(std::string_view name) const;

private:
    std::string_view path;

    std::unordered_map<std::string, LevelData> levels;
};

#endif