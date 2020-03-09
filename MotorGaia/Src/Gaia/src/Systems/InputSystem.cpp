#include "InputSystem.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/System.h>

// Initialization
void InputSystem::init()
{
    // MOUSE
    MOUSE_POSITION_X = 0;
    MOUSE_POSITION_Y = 0;

    MOUSE_BUTTON_LEFT.hold = false;
    MOUSE_BUTTON_RIGHT.hold = false;
    MOUSE_BUTTON_MIDDLE.hold = false;

    SDL_GetMouseState(&MOUSE_POSITION_X, &MOUSE_POSITION_Y);

    // CONTROLLER SYSTEM
    for (int i = 0; i < 4; i++) controllers[i].controllerIndex = i;

    if (!SDL_WasInit(SDL_INIT_JOYSTICK)) SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    SDL_GameControllerEventState(true);

}

// Closing
void InputSystem::close()
{
    // CONTROLLER SYSTEM
    for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS; ++ControllerIndex)
    {
        if (controllers[ControllerIndex].controller)
        {
            SDL_GameControllerClose(controllers[ControllerIndex].controller);
            if (controllers[ControllerIndex].controllerRumble) SDL_HapticClose(controllers[ControllerIndex].controllerRumble);
            controllers[ControllerIndex].isConected = false;
        }
    }

    destroy();
}

/// MAIN LOOP

void InputSystem::update()
{
    // Clear previous frame press/release events
    clearInputs();

    // Get new input events
    SDL_Event event;
    int index; int controllerIndex;
    std::string key;

    while (SDL_PollEvent(&event))
    {
        /*SDL_JoystickUpdate();*/
        switch (event.type)
        {
            // Keyboard events
        case SDL_KEYDOWN:
            key = SDL_GetKeyName(event.key.keysym.sym);
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
            if (!isKeyPressed(key)) {
                keyPress.emplace(key);
                keyHold.emplace(key);
                if (flags) std::cout << "Key down: " << key << "\n";
            }  
            break;
        case SDL_KEYUP:
            key = SDL_GetKeyName(event.key.keysym.sym);
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
            keyRelease.emplace(key);
            keyHold.erase(key);
            if (flags) std::cout << "Key up: " << key << "\n";
            break;
        case SDL_TEXTINPUT:
            break;

            // Mouse events
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&MOUSE_POSITION_X, &MOUSE_POSITION_Y);
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(
                static_cast<float>(MOUSE_POSITION_X),
                static_cast<float>(MOUSE_POSITION_Y)
            );
            break;
        case SDL_MOUSEBUTTONDOWN:
            processMouseInputDown(event.button);
            if(flags) std::cout << "Mouse X: " << MOUSE_POSITION_X << " | Mouse Y: " << MOUSE_POSITION_Y << "\n";
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseInputUp(event.button);
            break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                MOUSE_WHEEL = 1;
                if (flags) std::cout << "Mouse wheel up" << "\n";
            }
            else if (event.wheel.y < 0) {
                MOUSE_WHEEL = -1;
                if (flags) std::cout << "Mouse wheel down" << "\n";
            }
            break;

            // Controller events
        case SDL_CONTROLLERBUTTONDOWN:

            if ( (controllerIndex = getControllerFromEvent(&event) ) == -1) break;

            if (controllers[controllerIndex].isConected) controllerInputDown(controllerIndex);

            if (flags) std::cout << "Controller: " << controllerIndex << " Button Down" << "\n";

            break;
        case SDL_CONTROLLERBUTTONUP:

            if ((controllerIndex = getControllerFromEvent(&event)) == -1) break;

            if (controllers[controllerIndex].isConected) controllerInputUp(controllerIndex);

            if (flags) std::cout << "Controller: " << controllerIndex << " Button Up" << "\n";

            break;

        

        case SDL_CONTROLLERDEVICEADDED:
            if (currentControllers < MAX_CONTROLLERS && SDL_IsGameController(event.cdevice.which)) {

                index = event.cdevice.which;

                if ((controllerIndex = getFirstFreeController()) == -1) break;

                controllers[controllerIndex].controller = SDL_GameControllerOpen(index);
                controllers[controllerIndex].isConected = true;
                controllers[controllerIndex].ID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[controllerIndex].controller));

                 // Checks if controller has rumble device & asigns it
                SDL_Joystick* JoystickHandle = SDL_GameControllerGetJoystick(controllers[controllerIndex].controller);
                controllers[controllerIndex].controllerRumble = SDL_HapticOpenFromJoystick(JoystickHandle);

                if (SDL_HapticRumbleInit(controllers[controllerIndex].controllerRumble) != 0)
                {
                    SDL_HapticClose(controllers[controllerIndex].controllerRumble);
                }

                currentControllers++;

                if (flags) std::cout << "Controller: " << controllerIndex << " added" << "\n";
            }
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            index = event.cdevice.which;

            if (index < 0) return; // unknown controller?

            if ((controllerIndex = getControllerRemovedIndex(&event) ) == -1) break;

            SDL_GameControllerClose(controllers[controllerIndex].controller);

            if (controllers[controllerIndex].controllerRumble) {
                SDL_HapticClose(controllers[controllerIndex].controllerRumble);
                controllers[controllerIndex].controllerRumble = nullptr;
            }

            controllers[controllerIndex].controller = nullptr;
            controllers[controllerIndex].isConected = false;

            currentControllers--;

            if (flags) std::cout << "Controller: " << controllerIndex << " removed" << "\n";

            break;

        case SDL_CONTROLLERDEVICEREMAPPED:
            break;

            // System events
        case SDL_QUIT:
            exit = true;
            break;

        case SDL_WINDOWEVENT:
            break;

            // UNUSED
        case SDL_CONTROLLERAXISMOTION:
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
            break;
        case SDL_JOYBUTTONUP:
            break;
        case SDL_JOYDEVICEADDED:
            break;
        case SDL_JOYDEVICEREMOVED:
            break;
        case SDL_JOYHATMOTION:
            break;

            // Default
        default:
            if(flags) std::cout << "Unknown event: " << event.type <<"\n";
            break;
        }

        // Save current keyboard state
        keyboardState = SDL_GetKeyboardState(NULL);

        for (int i = 0; i < MAX_CONTROLLERS; i++)
        {
            if (controllers[i].isConected)
            {
                // Left joystick
                controllers[i].LeftStickX = abs(SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_LEFTX)) > controllers[i].JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_LEFTX) : 0;
                controllers[i].LeftStickY = abs(SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_LEFTY)) > controllers[i].JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_LEFTY) : 0;

                // Right joystick
                controllers[i].RightStickX = abs(SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_RIGHTX)) > controllers[i].JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_RIGHTX) : 0;
                controllers[i].RightStickY = abs(SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_RIGHTY)) > controllers[i].JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_RIGHTY) : 0;

                // Triggers
                controllers[i].LeftTrigger = SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                controllers[i].RightTrigger = SDL_GameControllerGetAxis(controllers[i].controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            }
            else
            {
                // TODO: This controller is not plugged in.
            }
        }

    }
}

void InputSystem::setDeadZone(int controller, int zone)
{
    if (controller > 0 && controller < 4)
        controllers[controller].JOYSTICK_DEAD_ZONE = zone;
    if(flags) std::cout << "Controller: " << controller << " | Deadzone: " << zone << "\n";
}

// KEYBOARD

/// Returns "true" while "key" is hold
bool InputSystem::isKeyPressed(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyHold.find(key) != keyHold.end();
    return is_in;
}

/// Returns "true" if "key" has been pressed on latest frame
bool InputSystem::getKeyPress(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyPress.find(key) != keyPress.end();
    return is_in;
}

/// Returns "true" if "key" has been released on latest frame
bool InputSystem::getKeyRelease(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyRelease.find(key) != keyRelease.end();
    return is_in;
}

// MOUSE

void InputSystem::processMouseInputDown (SDL_MouseButtonEvent& e)
{
    switch (e.button) {
    case SDL_BUTTON_LEFT:
        MOUSE_BUTTON_LEFT.hold = true;
        MOUSE_BUTTON_LEFT.pressed = true;

        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
        break;
    case SDL_BUTTON_RIGHT:
        MOUSE_BUTTON_RIGHT.hold = true;
        MOUSE_BUTTON_RIGHT.pressed = true;

        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
        break;
    case SDL_BUTTON_MIDDLE:
        MOUSE_BUTTON_MIDDLE.hold = true;
        MOUSE_BUTTON_MIDDLE.pressed = true;
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
        break;
    default:
        break;
    }
}

void InputSystem::processMouseInputUp(SDL_MouseButtonEvent& e)
{
    switch (e.button) {
    case SDL_BUTTON_LEFT:
        MOUSE_BUTTON_LEFT.hold = false;
        MOUSE_BUTTON_LEFT.released = true;
        break;
    case SDL_BUTTON_RIGHT:
        MOUSE_BUTTON_RIGHT.hold = false;
        MOUSE_BUTTON_RIGHT.released = true;
        break;
    case SDL_BUTTON_MIDDLE:
        MOUSE_BUTTON_MIDDLE.hold = false;
        MOUSE_BUTTON_MIDDLE.released = true;
        break;
    default:
        break;
    }
}

/// 'l' = left button | 'r' = right button | 'm' = middle button
bool InputSystem::getMouseButtonClick(char button)
{
    switch (button) {
    case 'l':
        return MOUSE_BUTTON_LEFT.pressed;
        break;
    case 'r':
        return MOUSE_BUTTON_RIGHT.pressed;
        break;
    case 'm':
        return MOUSE_BUTTON_MIDDLE.pressed;
        break;
    default:
        std::cout << "Button does not exist\n";
        ;
    }
    return false;
}

/// 'l' = left button | 'r' = right button | 'm' = middle button
bool InputSystem::getMouseButtonHold(char button)
{
    switch (button) {
    case 'l':
        return MOUSE_BUTTON_LEFT.hold;
        break;
    case 'r':
        return MOUSE_BUTTON_RIGHT.hold;
        break;
    case 'm':
        return MOUSE_BUTTON_MIDDLE.hold;
        break;
    default:
        std::cout << "Button does not exist\n";
        ;
    }
    return false;
}

/// 'l' = left button | 'r' = right button | 'm' = middle button
bool InputSystem::getMouseButtonRelease(char button)
{
    switch (button) {
    case 'l':
        return MOUSE_BUTTON_LEFT.released;
        break;
    case 'r':
        return MOUSE_BUTTON_RIGHT.released;
        break;
    case 'm':
        return MOUSE_BUTTON_MIDDLE.released;
        break;
    default:
        std::cout << "Button does not exist\n";
        ;
    }
    return false;
}


// CONTROLLER

void InputSystem::controllerInputDown(int index)
{
    if (!controllers[index].isConected)return;

    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_UP) && !controllers[index].Up) {
        controllers[index].Up = true;
        controllers[index].buttonPress.emplace("UP");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) && !controllers[index].Down) {
        controllers[index].Down = true;
        controllers[index].buttonPress.emplace("DOWN");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) && !controllers[index].Left) {
        controllers[index].Left = true;
        controllers[index].buttonPress.emplace("LEFT");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && !controllers[index].Right) {
        controllers[index].Right = true;
        controllers[index].buttonPress.emplace("RIGHT");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_START) && !controllers[index].Start) {
        controllers[index].Start = true;
        controllers[index].buttonPress.emplace("START");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_BACK) && !controllers[index].Back) {
        controllers[index].Back = true;
        controllers[index].buttonPress.emplace("BACK");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && !controllers[index].LeftShoulder) {
        controllers[index].LeftShoulder = true;
        controllers[index].buttonPress.emplace("LB");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && !controllers[index].RightShoulder) {
        controllers[index].RightShoulder = true;
        controllers[index].buttonPress.emplace("RB");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_A) && !controllers[index].AButton) {
        controllers[index].AButton = true;
        controllers[index].buttonPress.emplace("A");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_B) && !controllers[index].BButton) {
        controllers[index].BButton = true;
        controllers[index].buttonPress.emplace("B");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_X) && !controllers[index].XButton) {
        controllers[index].XButton = true;
        controllers[index].buttonPress.emplace("X");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_Y) && !controllers[index].YButton) {
        controllers[index].YButton = true;
        controllers[index].buttonPress.emplace("Y");
    }
}

void InputSystem::controllerInputUp(int index)
{
    if (!controllers[index].isConected)return;

    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_UP) && !controllers[index].Up) {
        controllers[index].Up = false;
        controllers[index].buttonRelease.emplace("UP");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) && !controllers[index].Down) {
        controllers[index].Down = false;
        controllers[index].buttonRelease.emplace("DOWN");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) && !controllers[index].Left) {
        controllers[index].Left = false;
        controllers[index].buttonRelease.emplace("LEFT");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && !controllers[index].Right) {
        controllers[index].Right = false;
        controllers[index].buttonRelease.emplace("RIGHT");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_START) && !controllers[index].Start) {
        controllers[index].Start = false;
        controllers[index].buttonRelease.emplace("START");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_BACK) && !controllers[index].Back) {
        controllers[index].Back = false;
        controllers[index].buttonRelease.emplace("BACK");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && !controllers[index].LeftShoulder) {
        controllers[index].LeftShoulder = false;
        controllers[index].buttonRelease.emplace("LB");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && !controllers[index].RightShoulder) {
        controllers[index].RightShoulder = false;
        controllers[index].buttonRelease.emplace("RB");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_A) && !controllers[index].AButton) {
        controllers[index].AButton = false;
        controllers[index].buttonRelease.emplace("A");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_B) && !controllers[index].BButton) {
        controllers[index].BButton = false;
        controllers[index].buttonRelease.emplace("B");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_X) && !controllers[index].XButton) {
        controllers[index].XButton = false;
        controllers[index].buttonRelease.emplace("X");
    }
    if (SDL_GameControllerGetButton(controllers[index].controller, SDL_CONTROLLER_BUTTON_Y) && !controllers[index].YButton) {
        controllers[index].YButton = false;
        controllers[index].buttonRelease.emplace("Y");
    }
}

bool InputSystem::isButtonPressed(int controllerIndex, std::string button)
{
    if (!controllers[controllerIndex].isConected) return false;

    std::transform(button.begin(), button.end(), button.begin(), ::toupper);

    if (button == "UP") {
        return controllers[controllerIndex].Up;
    }else if (button == "DOWN") {
        return controllers[controllerIndex].Down;
    }
    else if (button == "RIGHT") {
        return controllers[controllerIndex].Right;
    }
    else if (button == "LEFT") {
        return controllers[controllerIndex].Left;
    }
    else if (button == "START") {
        return controllers[controllerIndex].Start;
    }
    else if (button == "BACK") {
        return controllers[controllerIndex].Back;
    }
    else if (button == "LB") {
        return controllers[controllerIndex].LeftShoulder;
    }
    else if (button == "RB") {
        return controllers[controllerIndex].RightShoulder;
    }
    else if (button == "A") {
        return controllers[controllerIndex].AButton;
    }
    else if (button == "B") {
        return controllers[controllerIndex].BButton;
    }
    else if (button == "Y") {
        return controllers[controllerIndex].YButton;
    }
    else if (button == "X") {
        return controllers[controllerIndex].XButton;
    }
    // TODO
    else return false;
}

bool InputSystem::getButtonPress(int controllerIndex, std::string button)
{
    if (!controllers[controllerIndex].isConected) return false;

    std::transform(button.begin(), button.end(), button.begin(), ::toupper);
    const bool is_in = controllers[controllerIndex].buttonPress.find(button) != controllers[controllerIndex].buttonPress.end();
    return is_in;
}

bool InputSystem::getButtonRelease(int controllerIndex, std::string button)
{
    if (!controllers[controllerIndex].isConected) return false;

    std::transform(button.begin(), button.end(), button.begin(), ::toupper);
    const bool is_in = controllers[controllerIndex].buttonRelease.find(button) != controllers[controllerIndex].buttonRelease.end();
    return is_in;
}

/// strength = rumble strength percentage [0.0 , 1.0] | length = duration of effect in miliseconds
void InputSystem::controllerRumble(int controllerIndex, float strength, int length)
{
    if (strength > 1.0f) strength = 1.0f;
    if (length > 10000) length = 10000;
    if (controllers[controllerIndex].isConected && controllers[controllerIndex].controllerRumble)
    {
        SDL_HapticRumblePlay(controllers[controllerIndex].controllerRumble, strength, length);
    }
}
#pragma region UTILS
void InputSystem::clearInputs()
{
    keyPress.clear();
    keyRelease.clear();

    if (MOUSE_BUTTON_LEFT.pressed) MOUSE_BUTTON_LEFT.pressed = false;
    else if (MOUSE_BUTTON_LEFT.released)MOUSE_BUTTON_LEFT.released = false;

    if (MOUSE_BUTTON_RIGHT.pressed) MOUSE_BUTTON_RIGHT.pressed = false;
    else if (MOUSE_BUTTON_RIGHT.released)MOUSE_BUTTON_RIGHT.released = false;

    if (MOUSE_BUTTON_MIDDLE.pressed) MOUSE_BUTTON_MIDDLE.pressed = false;
    else if (MOUSE_BUTTON_MIDDLE.released)MOUSE_BUTTON_MIDDLE.released = false;

    MOUSE_WHEEL = 0;

    for (int i = 0; i < currentControllers; i++) {
        controllers[i].buttonPress.clear();
        controllers[i].buttonRelease.clear();
    }
}
int InputSystem::getFirstFreeController()
{
    for (int i = 0; i < 4; i++) {
        if (!controllers[i].isConected) return i;
    }
    return -1;
}
int InputSystem::getControllerByReference(SDL_GameController* handle)
{
    for (int i = 0; i < 4; i++) {
        if (controllers[i].controller == handle) return i;
    }
    return -1;
}
int InputSystem::getControllerFromEvent(SDL_Event* e)
{
    for (int i = 0; i < 4; i++) {
        if (controllers[i].ID == e->cbutton.which) return i;
    }
    return -1;
}
int InputSystem::getControllerRemovedIndex(SDL_Event* e)
{
    for (int i = 0; i < 4; i++) {
        if (controllers[i].ID == e->cdevice.which) return i;
    }
    return -1;
}
#pragma endregion
