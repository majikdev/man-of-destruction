#include "level_select_menu.h"
#include "game/level/level.h"
#include "game/level/level_list.h"
#include "game/menu/main_menu.h"
#include "game/save/save_slot.h"
#include <filesystem>

// Initialise the menu.

LevelSelectMenu::LevelSelectMenu()
    : Menu(13),
      currentPage(0)
{
    levels.reserve(levelCount);

    // Populate the level list.

    for (std::string_view name : levelList)
    {
        levels.emplace_back(name);
    }

    for (const auto& file : std::filesystem::directory_iterator("levels"))
    {
        if (file.path().extension() == ".level")
        {
            std::string name = file.path().stem().string();

            if (std::find(levels.begin(), levels.end(), name) == levels.end())
            {
                levels.push_back(name);
            }
        }
    }

    totalPages = ((int) levels.size() - 1) / 9 + 1;

    RefreshMenu();

    // Pressing escape goes back.

    SetEscapeCallback(std::bind(&LevelSelectMenu::OnPressBack, this));
}

// Press back callback.

void LevelSelectMenu::OnPressBack()
{
    Menu::Open<MainMenu>();
}

// Press previous callback.

void LevelSelectMenu::OnPressPrevious()
{
    if (currentPage > 0)
    {
        currentPage--;
        RefreshMenu();
    }
}

// Press next callback.

void LevelSelectMenu::OnPressNext()
{
    if (currentPage < totalPages - 1)
    {
        currentPage++;
        RefreshMenu();
    }
}

// Select level callback.

void LevelSelectMenu::OnSelectLevel(int index)
{
    std::string_view name = levels[currentPage * 9 + index];

    if (pSave->IsLevelUnlocked(name))
    {
        Level::Load(name);
        Menu::Close();
    }
}

// Refresh the menu widgets.

void LevelSelectMenu::RefreshMenu()
{
    ClearWidgets();

    // Page indicator.

    std::string pageText = "Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages);

    AddWidget(-6.25f, 4.25f, 2, nullptr, std::move(pageText), 0.0f);

    // Navigation buttons.

    AddWidget(-4.25f, -4.25f, 0, std::bind(&LevelSelectMenu::OnPressBack, this),     "Back");
    AddWidget(  0.0f, -4.25f, 0, std::bind(&LevelSelectMenu::OnPressPrevious, this), "Previous");
    AddWidget( 4.25f, -4.25f, 0, std::bind(&LevelSelectMenu::OnPressNext, this),     "Next");

    // Generate the level list.

    int beginning = currentPage * 9;
    int count = Min((int) levels.size() - beginning, 9);

    for (int i = 0; i < count; i++)
    {
        float x = (float) (i % 3 - 1) * 4.25f;
        float y = (float) (i / 3 - 1) * -2.25f;

        std::string name = levels[beginning + i];
        std::string text = Level::FormatName(name) + '\n';

        // Level status.

        if (pSave->IsLevelCompleted(name))
        {
            text += Level::TimeToString(pSave->GetLevelTime(name));
        }
        else if (pSave->IsLevelUnlocked(name))
        {
            text += "No time";
        }
        else
        {
            text += "Locked";
        }

        AddWidget(x, y, 1, std::bind(&LevelSelectMenu::OnSelectLevel, this, i), std::move(text), 0.0f);
    }
}