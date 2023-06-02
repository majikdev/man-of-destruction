#include "pause_menu.h"
#include "core/audio/sound_mixer.h"
#include "game/level/level.h"
#include "game/menu/main_menu.h"

// Initialise the menu.

PauseMenu::PauseMenu()
{
    AddSmallButton(0.0f,  1.25f, std::bind(&PauseMenu::OnPressResume, this), "Resume", 0.5f);
    AddSmallButton(0.0f,   0.0f, std::bind(&PauseMenu::OnPressRetry, this),  "Retry",  0.5f);
    AddSmallButton(0.0f, -1.25f, std::bind(&PauseMenu::OnPressQuit, this),   "Quit",   0.5f);

    // Pressing escape resumes the game.

    SetEscapeCallback(std::bind(&PauseMenu::OnPressResume, this));

    // Play an opening sound.

    pSoundMixer->PlaySound(pressSound);
}

// Press resume callback.

void PauseMenu::OnPressResume()
{
    Menu::Close();
}

// Press retry callback.

void PauseMenu::OnPressRetry()
{
    Level::Load(pLevel->GetName());
    Menu::Close();
}

// Press quit callback.

void PauseMenu::OnPressQuit()
{
    Level::Unload();
    Menu::Open<MainMenu>();
}