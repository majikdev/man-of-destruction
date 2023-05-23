#ifndef LEVEL_SELECT_MENU_H
#define LEVEL_SELECT_MENU_H

#include "menu.h"

class LevelSelectMenu : public Menu
{
public:
    LevelSelectMenu();

private:
    void OnPressBack();
    void OnPressPrevious();
    void OnPressNext();
    void OnSelectLevel(int index);

    void RefreshMenu();

private:
    int currentPage;
    int totalPages;

    std::vector<std::string> levels;
};

#endif