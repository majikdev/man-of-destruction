#ifndef MENU_H
#define MENU_H

#include "core/minimal.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

enum WidgetType
{
    BUTTON,
    STRING
};

struct Widget
{
    WidgetType type;
    vector2f position;
    vector2f bounds;

    // Button widget.

    std::function<void(int)> pOnPress;

    int spriteIndex;

    // String widget.

    std::string string;

    float alignment;
    float scrollTime;
    int maxLength;
};

extern std::shared_ptr<class Menu> pMenu;

class Menu
{
public:
    Menu();
    virtual ~Menu() = default;

    void Update(float delta);
    void Render() const;

protected:
    void AddSmallButton(float x, float y, std::function<void(int)> pOnPress, std::string string, float alignment);
    void AddLargeButton(float x, float y, std::function<void(int)> pOnPress, std::string string[2], float alignment);
    void AddString(float x, float y, std::string string, float alignment);
    void ClearWidgets();
    void SetEscapeCallback(std::function<void()> pCallback);

    int GetHoveredWidget(vector2f mousePosition) const;

public:
    template<class T, typename... Params>
    static void Open(Params... parameters);
    static void Close();

protected:
    std::vector<Widget> widgets;
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