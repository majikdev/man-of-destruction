#include "level_complete_menu.h"
#include "game/level/level.h"
#include "game/level/level_list.h"
#include "game/menu/main_menu.h"

// Initialise the menu.

LevelCompleteMenu::LevelCompleteMenu(int index, std::string_view name)
    : completedIndex(index)
{
    std::string text[2] = {Level::FormatName(std::string(name)), Level::TimeToString(pLevel->GetTime())};
    AddLargeButton(0.0f, 1.25f, nullptr, text, 0.0f);

    bool hasNext = (index < levelCount - 1);

    if (hasNext)
    {
        AddSmallButton(0.0f, -0.5f, std::bind(&LevelCompleteMenu::OnPressNext, this), "Next", 0.5f);
    }

    AddSmallButton(0.0f, hasNext ? -1.75f : -0.5f, std::bind(&LevelCompleteMenu::OnPressRetry, this), "Retry", 0.5f);
    AddSmallButton(0.0f, hasNext ? -3.0f : -1.75f, std::bind(&LevelCompleteMenu::OnPressQuit, this),  "Quit",  0.5f);
}

// Press next callback.

void LevelCompleteMenu::OnPressNext()
{
    Level::Load(levelList[completedIndex + 1]);
    Menu::Close();
}

// Press retry callback.

void LevelCompleteMenu::OnPressRetry()
{
    Level::Load(pLevel->GetName());
    Menu::Close();
}

// Press quit callback.

void LevelCompleteMenu::OnPressQuit()
{
    Level::Unload();
    Menu::Open<MainMenu>();
}