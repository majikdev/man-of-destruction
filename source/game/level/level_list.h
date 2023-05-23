#ifndef LEVEL_LIST_H
#define LEVEL_LIST_H

#include <string_view>

constexpr int levelCount = 9;

constexpr std::string_view levelList[]
{
    "level_1", "level_2", "level_3",
    "level_4", "level_5", "level_6",
    "level_7", "level_8", "level_9"
};

// Validate that level list length matches level count.

static_assert(sizeof(levelList) / sizeof(std::string_view) == levelCount);

#endif