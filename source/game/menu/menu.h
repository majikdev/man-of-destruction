#ifndef MENU_H
#define MENU_H

#include "core/minimal.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

struct WidgetType
{
    vector2f bounds;

    int spriteIndex;
};

struct Widget
{
    vector2f position;
    vector2f bounds;

    int spriteIndex;
    std::string text;
    float alignment;

    std::function<void(int)> pOnPress;
};

extern std::shared_ptr<class Menu> pMenu;

class Menu
{
public:
    Menu(int widgetCount);
    virtual ~Menu() = default;

    void Update(float delta);
    void Render() const;

protected:
    void AddWidget(float x, float y, int type, std::function<void(int)> pOnPress, std::string text, float alignment = 0.5f);
    void ClearWidgets();
    void SetEscapeCallback(std::function<void()> pCallback);

    int GetHoveredWidget(vector2f mousePosition) const;

public:
    template<class T, typename... Params>
    static void Open(Params... parameters);
    static void Close();

protected:
    std::vector<Widget> widgets;
    WidgetType widgetTypes[3];
    std::function<void()> pOnEscape;

    int hoveredWidget;

    Sprite sprites[4];
    Sound hoverSound;
    Sound pressSound;
};

// Open a menu of class.

template<class T, typename... Params>
void Menu::Open(Params... parameters)
{
    static_assert(std::is_base_of<Menu, T>::value);

    new T(parameters...);
}

#endif