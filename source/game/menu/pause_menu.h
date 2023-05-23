#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "menu.h"

class PauseMenu : public Menu
{
public:
    PauseMenu();

private:
    void OnPressResume();
    void OnPressRetry();
    void OnPressQuit();
};

#endif