#ifndef LEVEL_COMPLETE_MENU_H
#define LEVEL_COMPLETE_MENU_H

#include "menu.h"
#include <string_view>

class LevelCompleteMenu : public Menu
{
public:
    LevelCompleteMenu(int index, std::string_view name);

private:
    void OnPressNext();
    void OnPressRetry();
    void OnPressQuit();

    int completedIndex;
};

#endif