#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <string_view>

struct GLFWwindow;

struct WindowData
{
    std::function<void(int, int)> pOnKeyboardKey;
    std::function<void(int, int)> pOnMouseButton;
    std::function<void(int, int)> pOnMousePosition;
    std::function<void(int, int)> pOnResizeWindow;

    int width;
    int height;
    bool fullscreen;
};

extern class Window* pWindow;

class Window
{
public:
    Window(int width, int height, std::string_view title);
    ~Window();

    void Update();

    void SetKeyboardKeyCallback(std::function<void(int button, int action)> pCallback);
    void SetMouseButtonCallback(std::function<void(int button, int action)> pCallback);
    void SetMousePositionCallback(std::function<void(int x, int y)> pCallback);
    void SetResizeCallback(std::function<void(int width, int height)> pCallback);
    void ToggleFullscreen();
    void Close();

    int GetWidth() const;
    int GetHeight() const;
    bool IsFullscreen() const;
    bool IsOpen() const;

private:
    WindowData data;

    GLFWwindow* pNativeWindow;
};

#endif