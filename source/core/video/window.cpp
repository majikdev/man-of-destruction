#define GLFW_INCLUDE_NONE

#include "window.h"

#include <utility>
#include "core/logging.h"
#include "glfw/glfw3.h"

Window* pWindow;

// Initialise the window.

Window::Window(int width, int height, std::string_view title)
    : data{nullptr, nullptr, nullptr, nullptr, width, height, width, height, false}
{
    pWindow = this;

    // Initialise GLFW and use OpenGL 3.3 CORE.

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and make it the current context.

    pNativeWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!pNativeWindow)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(pNativeWindow);

    // Set the window's size limits and enable VSync.

    glfwSetWindowSizeLimits(pNativeWindow, 960, 720, -1, -1);
    glfwSwapInterval(1);

    LOG("Initialised the Window.");
}

// Terminate the window.

Window::~Window()
{
    glfwDestroyWindow(pNativeWindow);
    glfwTerminate();
}

// Swap buffers and poll events.

void Window::Update()
{
    glfwSwapBuffers(pNativeWindow);
    glfwPollEvents();
}

// Set a callback for keyboard input.

void Window::SetKeyboardKeyCallback(std::function<void(int button, int action)> pCallback)
{
    data.pOnKeyboardKey = std::move(pCallback);
    glfwSetWindowUserPointer(pNativeWindow, (void*) &data);

    // After passing a function in WindowData to GLFW, call it from the callback lambda.

    glfwSetKeyCallback(pNativeWindow, [](GLFWwindow* pWindow, int button, int code, int action, int mods)
    {
        auto pData = (WindowData*) glfwGetWindowUserPointer(pWindow);
        pData->pOnKeyboardKey(button, action);
    });
}

// Set a callback for mouse input.

void Window::SetMouseButtonCallback(std::function<void(int button, int action)> pCallback)
{
    data.pOnMouseButton = std::move(pCallback);
    glfwSetWindowUserPointer(pNativeWindow, (void*) &data);

    // After passing a function in WindowData to GLFW, call it from the callback lambda.

    glfwSetMouseButtonCallback(pNativeWindow, [](GLFWwindow* pWindow, int button, int action, int mods)
    {
        auto pData = (WindowData*) glfwGetWindowUserPointer(pWindow);
        pData->pOnMouseButton(button, action);
    });
}

// Set a callback for mouse position.

void Window::SetMousePositionCallback(std::function<void(int x, int y)> pCallback)
{
    data.pOnMousePosition = std::move(pCallback);
    glfwSetWindowUserPointer(pNativeWindow, (void*) &data);

    // After passing a function in WindowData to GLFW, call it from the callback lambda.

    glfwSetCursorPosCallback(pNativeWindow, [](GLFWwindow* pWindow, double x, double y)
    {
        auto pData = (WindowData*) glfwGetWindowUserPointer(pWindow);
        pData->pOnMousePosition((int) x, (int) y);
    });
}

// Set a callback for window resizing.

void Window::SetResizeCallback(std::function<void(int width, int height)> pCallback)
{
    data.pOnResizeWindow = std::move(pCallback);
    glfwSetWindowUserPointer(pNativeWindow, (void*) &data);

    // After passing a function in WindowData to GLFW, call it from the callback lambda.
    // Along with calling the window resize callback, set the WindowData's dimensions.

    glfwSetWindowSizeCallback(pNativeWindow, [](GLFWwindow* pWindow, int width, int height)
    {
        // Round up to even to avoid UV bleeding artifacts.
        width = (width + 1) / 2 * 2;
        height = (height + 1) / 2 * 2;

        auto pData = (WindowData*) glfwGetWindowUserPointer(pWindow);
        pData->pOnResizeWindow(width, height);

        // Save the new window dimensions.

        pData->width = width;
        pData->height = height;

        // Set the new desired width and height.

        if (!pData->fullscreen)
        {
            pData->desiredWidth = width;
            pData->desiredHeight = height;
        }
    });
}

// Toggle the window's fullscreen mode.

void Window::ToggleFullscreen()
{
    data.fullscreen = !data.fullscreen;

    if (data.fullscreen)
    {
        // Enable fullscreen mode.

        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);

        glfwSetWindowMonitor(pNativeWindow, pMonitor, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
        data.fullscreen = true;
    }
    else
    {
        // Disable fullscreen mode.

        GLFWmonitor* pMonitor = glfwGetWindowMonitor(pNativeWindow);
        const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);

        glfwSetWindowMonitor(pNativeWindow, nullptr, 100, 100, data.desiredWidth, data.desiredHeight, pMode->refreshRate);
        data.fullscreen = false;
    }

    // Re-enable VSync as it is reset when changing window mode.

    glfwSwapInterval(1);
}

// Close the window.

void Window::Close()
{
    glfwSetWindowShouldClose(pNativeWindow, 1);
}

// Get the window's width.

int Window::GetWidth() const
{
    return data.width;
}

// Get the window's height.

int Window::GetHeight() const
{
    return data.height;
}

// Check if the window is fullscreen.

bool Window::IsFullscreen() const
{
    return data.fullscreen;
}

// Check if the window is open.

bool Window::IsOpen() const
{
    return !glfwWindowShouldClose(pNativeWindow);
}