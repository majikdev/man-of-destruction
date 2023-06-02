#include "menu.h"
#include "core/audio/sound_mixer.h"
#include "core/input/controller.h"
#include "core/video/renderer.h"
#include "game/camera/camera.h"

std::shared_ptr<Menu> pMenu;

// Initialise the menu.

Menu::Menu()
    : hoveredWidget(-1), sprites(), hoverSound(), pressSound()
{
    pMenu.reset(this);

    // Load the necessary resources.

    SpriteSheet sheet = pRenderer->GetSheet("assets/sprites/widget/widget.bmp");
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

    // Scroll the string widgets.

    for (Widget& widget : widgets)
    {
        int stringLength = (int) widget.string.length();

        if (widget.type == STRING && widget.maxLength != 0 && stringLength > widget.maxLength)
        {
            widget.scrollTime += delta * 2.0f;

            if (widget.scrollTime >= (float) (stringLength - widget.maxLength + 1))
            {
                widget.scrollTime = 0.0f;
            }
        }
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

        if (widget.type == BUTTON)
        {
            const Sprite& sprite = sprites[i == hoveredWidget ? widget.spriteIndex + 1 : widget.spriteIndex];

            pRenderer->DrawSprite(sprite, widget.position.x, widget.position.y, 3.5f, widget.bounds.x, widget.bounds.y);
        }

        // Draw the widget's string.

        else
        {
            std::string string = widget.maxLength == 0 ? widget.string : widget.string.substr((int) widget.scrollTime, widget.maxLength);
            float x = widget.position.x + Clamp(widget.bounds.x * widget.alignment, 0.25f, widget.bounds.x - 0.25f);
            float y = widget.position.y + widget.bounds.y;

            pRenderer->DrawString(string, x, y, 4.0f, widget.alignment);
        }
    }
}

// Add a small button with a one-line string.

void Menu::AddSmallButton(float x, float y, std::function<void(int)> pOnPress, std::string string, float alignment)
{
    vector2f position(x, y);
    vector2f bounds(4.0f, 1.0f);

    widgets.reserve(2);

    position -= bounds * 0.5f;
    widgets.push_back({BUTTON, position, bounds, std::move(pOnPress), 0, "", 0.0f, 0.0f, 0});

    // One line of text.

    position.y -= 0.25f;
    widgets.push_back({STRING, position, bounds, nullptr, 0, std::move(string), alignment, 0.0f, 9});
}

// Add a large button with a two-line string.

void Menu::AddLargeButton(float x, float y, std::function<void(int)> pOnPress, std::string string[2], float alignment)
{
    vector2f position(x, y);
    vector2f bounds(4.0f, 2.0f);

    widgets.reserve(3);

    position -= bounds * 0.5f;
    widgets.push_back({BUTTON, position, bounds, std::move(pOnPress), 2, "", 0.0f, 0.0f, 0});

    // Two lines of text.

    position.y -= 0.375f;
    widgets.push_back({STRING, position, bounds, nullptr, 0, std::move(string[0]), alignment, 0.0f, 9});

    position.y -= 0.75f;
    widgets.push_back({STRING, position, bounds, nullptr, 0, std::move(string[1]), alignment, 0.0f, 9});
}

// Add a string widget.

void Menu::AddString(float x, float y, std::string string, float alignment)
{
    vector2f position(x, y - 0.25f);
    vector2f bounds(0.0f, 1.0f);

    widgets.push_back({STRING, position, bounds, nullptr, 0, std::move(string), alignment, 0.0f, 0});
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