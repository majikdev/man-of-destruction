#include "controller.h"
#include "core/logging.h"

Controller* pController;

// Initialise the controller.

Controller::Controller()
{
    pController = this;

    LOG("Initialised the Controller.");
}

// Update the state of a button.

void Controller::OnButtonAction(Button button, Action action)
{
    if (action == PRESS)
    {
        buttonStates[button] = true;
    }
    else if (action == RELEASE)
    {
        buttonStates.erase(button);
    }
}

// Update the screen-space mouse position.

void Controller::OnMousePosition(vector2f position)
{
    mousePosition = position;
}

// Check if a button is held down.

bool Controller::IsHeldDown(Button button) const
{
    return buttonStates.find(button) != buttonStates.end();
}

// Check if a button was just pressed.

bool Controller::WasPressed(Button button)
{
    if (buttonStates.find(button) != buttonStates.end())
    {
        // Return and update the button's 'just pressed' state.

        bool wasPressed = buttonStates[button];
        buttonStates[button] = false;

        return wasPressed;
    }

    // The button is not being held down.

    return false;
}

// Get the screen-space mouse position.

vector2f Controller::GetMousePosition() const
{
    return mousePosition;
}