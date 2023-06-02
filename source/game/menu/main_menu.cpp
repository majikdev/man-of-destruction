#include "main_menu.h"
#include "core/video/window.h"
#include "game/menu/level_select_menu.h"

// Initialise the menu.

MainMenu::MainMenu()
{
    AddString(0.0f, 1.25f, "MAN OF DESTRUCTION", 0.5f);

    AddSmallButton(0.0f,   0.0f, std::bind(&MainMenu::OnPressPlay, this), "Play", 0.5f);
    AddSmallButton(0.0f, -1.25f, std::bind(&MainMenu::OnPressQuit, this), "Quit", 0.5f);
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