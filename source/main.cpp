#include "core/audio/sound_mixer.h"
#include "core/input/controller.h"
#include "core/video/renderer.h"
#include "core/video/window.h"
#include "game/camera/camera.h"
#include "game/config/configuration.h"
#include "game/entity/player.h"
#include "game/level/level.h"
#include "game/menu/main_menu.h"
#include "game/save/save_slot.h"
#include <chrono>

// Button action callback.

static void OnButton(int button, int action)
{
    if (button == KEY_F11 && action == PRESS)
    {
        pWindow->ToggleFullscreen();
    }

    pController->OnButtonAction((Button) button, (Action) action);
}

// Mouse move callback.

static void OnMouse(int x, int y)
{
    float unitScale = (float) pConfig->pixelScale * 16.0f;

    vector2f position;
    position.x = ((float) x - (float) pWindow->GetWidth() / 2.0f) / unitScale;
    position.y = ((float) pWindow->GetHeight() / 2.0f - (float) y) / unitScale;

    pController->OnMousePosition(position);
}

// Window resize callback.

static void OnResize(int width, int height)
{
    pRenderer->SetResolution(width, height);
}

// Program entry point.

int main()
{
    START_METRIC("startup");

    Configuration config("configuration.toml");
    SaveSlot save(config.saveSlot);

    // Initialise the core (engine) subsystems.

    Window window(config.windowWidth, config.windowHeight, "Man of Destruction");
    window.SetKeyboardKeyCallback(OnButton);
    window.SetMouseButtonCallback(OnButton);
    window.SetMousePositionCallback(OnMouse);
    window.SetResizeCallback(OnResize);

    Renderer renderer("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    renderer.SetResolution(window.GetWidth(), window.GetHeight());
    renderer.SetFontSheet(renderer.GetSheet("assets/sprites/widget/font.bmp"));

    SoundMixer soundMixer;
    soundMixer.SetMasterVolume(config.masterVolume);

    Controller controller;

    Camera camera;
    camera.SetUnitScale(config.pixelScale * 16);
    camera.SetShakeStrength(config.cameraShake);

    // Set fullscreen mode based on config.

    if (config.fullscreen)
    {
        window.ToggleFullscreen();
    }

    STOP_METRIC("startup");

    // Begin the game and its loop.

    Menu::Open<MainMenu>();

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (window.IsOpen())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration<float>(currentTime - lastTime).count();

        // Calculate the delta time (time between the previous and current frame);
        // Below 40 fps, the game will slow down to retain accurate physics.

        delta = Min(delta, 0.025f);
        lastTime = currentTime;

#if _DEBUG
        std::string_view metric = pLevel ? "loop_level" : "loop_menu";
#endif

        START_METRIC(metric);

        // Update the game's logic.

        camera.Update(delta);

        if (pMenu)
        {
            pMenu->Update(delta);
        }
        else if (pLevel)
        {
            pLevel->Update(delta);
        }

        // Render the game's graphics.

        renderer.Clear();

        if (pLevel)
        {
            pLevel->Render();
        }

        if (pMenu)
        {
            pMenu->Render();
        }

        STOP_METRIC(metric);

        window.Update();
    }

    // Save the current fullscreen mode to config.

    config.fullscreen = window.IsFullscreen();

    SAVE_METRICS("metrics.txt");

    return 0;
}