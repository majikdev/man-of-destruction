#include "menu.h"
#include "core/audio/sound_mixer.h"
#include "core/input/controller.h"
#include "core/video/renderer.h"
#include "game/camera/camera.h"

std::shared_ptr<Menu> pMenu;

// Initialise the menu.

Menu::Menu(int widgetCount)
    : hoveredWidget(-1), sprites(), hoverSound(), pressSound()
{
    pMenu.reset(this);

    widgets.reserve(widgetCount);

    // Register the widget types.

    widgetTypes[0] = {vector2f(4.0f, 1.0f), 0};  // Small button.
    widgetTypes[1] = {vector2f(4.0f, 2.0f), 2};  // Big button.
    widgetTypes[2] = {vector2f(0.0f, 1.0f), -1}; // String.

    // Load the necessary resources.

    SpriteSheet sheet = pRenderer->GetSheet("assets/sprites/widgets/widgets.bmp");
    sprites[0] = sheet.GetSprite(0, 0, 64, 16);
    sprites[1] = sheet.GetSprite(64, 0, 64, 16);
    sprites[2] = sheet.GetSprite(0, 16, 64, 32);
    sprites[3] = sheet.GetSprite(64, 16, 64, 32);

    hoverSound = pSoundMixer->GetSound("assets/sounds/button_hover.wav");
    pressSound = pSoundMixer->GetSound("assets/sounds/button_press.wav");
}

// Update the menu.

void Menu::Update(float delta)
{
    int currentHover = GetHoveredWidget(pController->GetMousePosition());

    // Play a sound when hovering a widget.

    if (currentHover != hoveredWidget)
    {
        hoveredWidget = currentHover;

        if (hoveredWidget != -1)
        {
            pSoundMixer->PlaySound(hoverSound);
        }
    }

    // Press the currently hovered widget.

    if (pController->WasPressed(MOUSE_LEFT))
    {
        if (hoveredWidget != -1)
        {
            pSoundMixer->PlaySound(pressSound);
            widgets[hoveredWidget].pOnPress(hoveredWidget);
        }
    }

    // Press escape for a callback.

    if (pController->WasPressed(KEY_ESCAPE) && pOnEscape)
    {
        pSoundMixer->PlaySound(pressSound);
        pOnEscape();
    }
}

// Render the menu.

void Menu::Render() const
{
    pCamera->ApplyScreenProjection();

    // Draw all widgets.

    for (int i = 0; i < widgets.size(); i++)
    {
        const Widget& widget = widgets[i];

        // Draw the widget's sprite if applicable.

        if (widget.spriteIndex >= 0)
        {
            const Sprite& sprite = sprites[i == hoveredWidget ? widget.spriteIndex + 1 : widget.spriteIndex];

            pRenderer->DrawSprite(sprite, widget.position.x, widget.position.y, 3.5f, widget.bounds.x, widget.bounds.y);
        }

        // Draw the widget's string.

        float x = widget.position.x + Clamp(widget.bounds.x * widget.alignment, 0.25f, widget.bounds.x - 0.25f);
        float y = widget.position.y + widget.bounds.y - 0.25f;

        pRenderer->DrawString(widget.text, x, y, 4.0f, widget.alignment);
    }
}

// Add a widget to the menu.
// Alignment: 0.0 = left, 0.5 = centred, 1.0 = right.

void Menu::AddWidget(float x, float y, int type, std::function<void(int)> pOnPress, std::string text, float alignment)
{
    vector2f bounds = widgetTypes[type].bounds;
    vector2f position(x, y);

    position -= bounds * 0.5f;

    widgets.push_back({position, bounds, widgetTypes[type].spriteIndex, std::move(text), alignment, std::move(pOnPress)});
}

// Clear the menu widgets.

void Menu::ClearWidgets()
{
    widgets.clear();
}

// Set the escape key callback.

void Menu::SetEscapeCallback(std::function<void()> pCallback)
{
    pOnEscape = std::move(pCallback);
}

// Get the currently hovered widget.

int Menu::GetHoveredWidget(vector2f mousePosition) const
{
    float x = mousePosition.x;
    float y = mousePosition.y;

    for (int i = 0; i < widgets.size(); i++)
    {
        const Widget& widget = widgets[i];

        if (widget.pOnPress &&
            x >= widget.position.x && x <= widget.position.x + widget.bounds.x &&
            y >= widget.position.y && y <= widget.position.y + widget.bounds.y)
        {
            return i;
        }
    }

    return -1;
}

// Close the current menu.

void Menu::Close()
{
    pMenu.reset();
}