#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "menu.h"

class MainMenu : public Menu
{
public:
    MainMenu();

private:
    void OnPressPlay();
    void OnPressQuit();
};

#endif