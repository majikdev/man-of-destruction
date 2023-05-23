#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "action.h"
#include "button.h"
#include "core/maths/maths.h"
#include <unordered_map>

extern class Controller* pController;

class Controller
{
public:
    Controller();

    void OnButtonAction(Button button, Action action);
    void OnMousePosition(vector2f position);

    bool IsHeldDown(Button button) const;
    bool WasPressed(Button button);
    vector2f GetMousePosition() const;

private:
    std::unordered_map<Button, bool> buttonStates;
    vector2f mousePosition;
};

#endif