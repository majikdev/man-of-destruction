#include "level_complete_menu.h"
#include "game/level/level.h"
#include "game/level/level_list.h"
#include "game/menu/main_menu.h"

// Initialise the menu.

LevelCompleteMenu::LevelCompleteMenu(int index, std::string_view name)
    : Menu(3),
      completedIndex(index)
{
    AddWidget(0.0f,  1.25f, 1, nullptr, Level::FormatName(std::string(name)) + '\n' + Level::TimeToString(pLevel->GetTime()), 0.0f);

    bool hasNext = (index < levelCount - 1);

    if (hasNext)
    {
        AddWidget(0.0f, -0.5f, 0, std::bind(&LevelCompleteMenu::OnPressNext, this), "Next");
    }

    AddWidget(0.0f, hasNext ? -1.75f : -0.5f, 0, std::bind(&LevelCompleteMenu::OnPressRetry, this), "Retry");
    AddWidget(0.0f, hasNext ? -3.0f : -1.75f, 0, std::bind(&LevelCompleteMenu::OnPressQuit, this),  "Quit");
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