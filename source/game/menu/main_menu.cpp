#include "main_menu.h"
#include "core/video/window.h"
#include "game/menu/level_select_menu.h"

// Initialise the menu.

MainMenu::MainMenu()
    : Menu(3)
{
    AddWidget(0.0f,  1.25f, 2, nullptr, "MAN OF DESTRUCTION");

    AddWidget(0.0f,   0.0f, 0, std::bind(&MainMenu::OnPressPlay, this), "Play");
    AddWidget(0.0f, -1.25f, 0, std::bind(&MainMenu::OnPressQuit, this), "Quit");
}

// Press play callback.

void MainMenu::OnPressPlay()
{
    Menu::Open<LevelSelectMenu>();
}

// Press quit callback.

void MainMenu::OnPressQuit()
{
    pWindow->Close();
}