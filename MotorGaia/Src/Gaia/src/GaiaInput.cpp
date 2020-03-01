#include "GaiaInput.h"

// Initialization
void GaiaInput::init()
{
    // MOUSE
    MOUSE_POSITION_X = 0;
    MOUSE_POSITION_Y = 0;

    MOUSE_BUTTON_LEFT.hold = false;
    MOUSE_BUTTON_RIGHT.hold = false;
    MOUSE_BUTTON_MIDDLE.hold = false;

    SDL_GetMouseState(&MOUSE_POSITION_X, &MOUSE_POSITION_Y);

    // CONTROLLER SYSTEM
    int MaxJoysticks = SDL_NumJoysticks();
    int ControllerIndex = 0;
    for (int JoystickIndex = 0; JoystickIndex < MaxJoysticks; ++JoystickIndex)
    {
        if (!SDL_IsGameController(JoystickIndex))
        {
            continue;
        }
        if (ControllerIndex >= MAX_CONTROLLERS)
        {
            break;
        }
        ControllerHandles[ControllerIndex] = SDL_GameControllerOpen(JoystickIndex);
        ControllerIndex++;
        currentControllers++;

        // Checks if controller has rumble device & asigns it
        SDL_Joystick* JoystickHandle = SDL_GameControllerGetJoystick(ControllerHandles[ControllerIndex]);
        RumbleHandles[ControllerIndex] = SDL_HapticOpenFromJoystick(JoystickHandle);

        // Checks if rumble device is compatible with basic rumble features
        if (SDL_HapticRumbleInit(RumbleHandles[ControllerIndex]) != 0)
        {
            SDL_HapticClose(RumbleHandles[ControllerIndex]);
            RumbleHandles[ControllerIndex] = 0;
        }
    }
}

// Closing
void GaiaInput::close()
{

    // CONTROLLER SYSTEM
    for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS; ++ControllerIndex)
    {
        if (ControllerHandles[ControllerIndex])
        {
            SDL_GameControllerClose(ControllerHandles[ControllerIndex]);
            if (RumbleHandles[ControllerIndex]) SDL_HapticClose(RumbleHandles[ControllerIndex]);
        }
    }


}

/// MAIN LOOP

void GaiaInput::Update()
{
    // Clear previous frame press/release events
    clearInputs();

    // Get new input events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        std::string key;
        switch (event.type)
        {
            // Keyboard events
        case SDL_KEYDOWN:
            key = SDL_GetKeyName(event.key.keysym.sym);
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
            keyPress.emplace(key);
            keyHold.emplace(key);
            break;
        case SDL_KEYUP:
            key = SDL_GetKeyName(event.key.keysym.sym);
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
            keyRelease.emplace(key);
            keyHold.erase(key);
            break;

            // Mouse events
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&MOUSE_POSITION_X, &MOUSE_POSITION_Y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            processMouseInputDown(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseInputUp(event.button);
            break;

            // Controller events
        case SDL_CONTROLLERBUTTONDOWN:
            int index = event.cdevice.which;
            if (ControllerHandles[index] != 0 && SDL_GameControllerGetAttached(ControllerHandles[index])) controllerInputDown(index);
            break;
        case SDL_CONTROLLERBUTTONUP: {
            if (ControllerHandles[index] != 0 && SDL_GameControllerGetAttached(ControllerHandles[index])) controllerInputUp(index);
            break;
        }


        case SDL_CONTROLLERDEVICEADDED:
            if (event.cdevice.which < MAX_CONTROLLERS && SDL_IsGameController(event.cdevice.which)) {

                int index = event.cdevice.which;

                ControllerHandles[index] = SDL_GameControllerOpen(index);
                currentControllers++;

                // Checks if controller has rumble device & asigns it
                SDL_Joystick* JoystickHandle = SDL_GameControllerGetJoystick(ControllerHandles[index]);
                RumbleHandles[index] = SDL_HapticOpenFromJoystick(JoystickHandle);

                // Checks if rumble device is compatible with basic rumble features
                if (SDL_HapticRumbleInit(RumbleHandles[index]) != 0)
                {
                    SDL_HapticClose(RumbleHandles[index]);
                    RumbleHandles[index] = 0;
                }
            }
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            int index = event.cdevice.which;

            if (index < 0) return; // unknown controller?

            SDL_GameControllerClose(ControllerHandles[index]);
            if (RumbleHandles[index]) SDL_HapticClose(RumbleHandles[index]);

            break;
        case SDL_CONTROLLERDEVICEREMAPPED:
            break;

            // System events
        case SDL_QUIT:
            break;
        default:
            std::cout << "Unknown event\n";
        }

        // Save current keyboard state
        keyboardState = SDL_GetKeyboardState(NULL);

        for (int ControllerIndex = 0;
            ControllerIndex < MAX_CONTROLLERS;
            ++ControllerIndex)
        {
            if (ControllerHandles[ControllerIndex] != 0 && SDL_GameControllerGetAttached(ControllerHandles[ControllerIndex]))
            {
                // Left joystick
                controllers[ControllerIndex].LeftStickX = abs(SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTX)) > JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTX) : 0;
                controllers[ControllerIndex].LeftStickY = abs(SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTY)) > JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTY) : 0;

                // Right joystick
                controllers[ControllerIndex].RightStickX = abs(SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTX)) > JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTX) : 0;
                controllers[ControllerIndex].RightStickY = abs(SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTY)) > JOYSTICK_DEAD_ZONE ?
                    SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_RIGHTY) : 0;

                // Triggers
                controllers[ControllerIndex].LeftTrigger = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                controllers[ControllerIndex].RightTrigger = SDL_GameControllerGetAxis(ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            }
            else
            {
                // TODO: This controller is not plugged in.
            }
        }

    }
}

// KEYBOARD

/// Returns "true" while "key" is hold
bool GaiaInput::isKeyPressed(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyHold.find(key) != keyHold.end();
    return is_in;
}

/// Returns "true" if "key" has been pressed on latest frame
bool GaiaInput::getKeyPress(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyPress.find(key) != keyPress.end();
    return is_in;
}

/// Returns "true" if "key" has been released on latest frame
bool GaiaInput::getKeyRelease(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    const bool is_in = keyRelease.find(key) != keyRelease.end();
    return is_in;
}

// MOUSE

void GaiaInput::processMouseInputDown (SDL_MouseButtonEvent& e)
{
    switch (e.button) {
    case SDL_BUTTON_LEFT:
        MOUSE_BUTTON_LEFT.hold = true;
        MOUSE_BUTTON_LEFT.pressed = true;
        break;
    case SDL_BUTTON_RIGHT:
        MOUSE_BUTTON_RIGHT.hold = true;
        MOUSE_BUTTON_RIGHT.pressed = true;
        break;
    case SDL_BUTTON_MIDDLE:
        MOUSE_BUTTON_MIDDLE.hold = true;
        MOUSE_BUTTON_MIDDLE.pressed = true;
        break;
    default:
        break;
    }
}

void GaiaInput::processMouseInputUp(SDL_MouseButtonEvent& e)
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
bool GaiaInput::getMouseButtonClick(char button)
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
bool GaiaInput::getMouseButtonHold(char button)
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
bool GaiaInput::getMouseButtonRelease(char button)
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

void GaiaInput::controllerInputDown(int index)
{
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_UP) && !controllers[index].Up) {
        controllers[index].Up = true;
        controllers[index].buttonPress.emplace("UP");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_DOWN) && !controllers[index].Down) {
        controllers[index].Down = true;
        controllers[index].buttonPress.emplace("DOWN");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_LEFT) && !controllers[index].Left) {
        controllers[index].Left = true;
        controllers[index].buttonPress.emplace("LEFT");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && !controllers[index].Right) {
        controllers[index].Right = true;
        controllers[index].buttonPress.emplace("RIGHT");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_START) && !controllers[index].Start) {
        controllers[index].Start = true;
        controllers[index].buttonPress.emplace("START");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_BACK) && !controllers[index].Back) {
        controllers[index].Back = true;
        controllers[index].buttonPress.emplace("BACK");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && !controllers[index].LeftShoulder) {
        controllers[index].LeftShoulder = true;
        controllers[index].buttonPress.emplace("LB");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && !controllers[index].RightShoulder) {
        controllers[index].RightShoulder = true;
        controllers[index].buttonPress.emplace("RB");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_A) && !controllers[index].AButton) {
        controllers[index].AButton = true;
        controllers[index].buttonPress.emplace("A");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_B) && !controllers[index].BButton) {
        controllers[index].BButton = true;
        controllers[index].buttonPress.emplace("B");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_X) && !controllers[index].XButton) {
        controllers[index].XButton = true;
        controllers[index].buttonPress.emplace("X");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_Y) && !controllers[index].YButton) {
        controllers[index].YButton = true;
        controllers[index].buttonPress.emplace("Y");
    }
}

void GaiaInput::controllerInputUp(int index)
{
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_UP) && !controllers[index].Up) {
        controllers[index].Up = false;
        controllers[index].buttonRelease.emplace("UP");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_DOWN) && !controllers[index].Down) {
        controllers[index].Down = false;
        controllers[index].buttonRelease.emplace("DOWN");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_LEFT) && !controllers[index].Left) {
        controllers[index].Left = false;
        controllers[index].buttonRelease.emplace("LEFT");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && !controllers[index].Right) {
        controllers[index].Right = false;
        controllers[index].buttonRelease.emplace("RIGHT");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_START) && !controllers[index].Start) {
        controllers[index].Start = false;
        controllers[index].buttonRelease.emplace("START");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_BACK) && !controllers[index].Back) {
        controllers[index].Back = false;
        controllers[index].buttonRelease.emplace("BACK");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && !controllers[index].LeftShoulder) {
        controllers[index].LeftShoulder = false;
        controllers[index].buttonRelease.emplace("LB");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && !controllers[index].RightShoulder) {
        controllers[index].RightShoulder = false;
        controllers[index].buttonRelease.emplace("RB");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_A) && !controllers[index].AButton) {
        controllers[index].AButton = false;
        controllers[index].buttonRelease.emplace("A");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_B) && !controllers[index].BButton) {
        controllers[index].BButton = false;
        controllers[index].buttonRelease.emplace("B");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_X) && !controllers[index].XButton) {
        controllers[index].XButton = false;
        controllers[index].buttonRelease.emplace("X");
    }
    if (SDL_GameControllerGetButton(ControllerHandles[index], SDL_CONTROLLER_BUTTON_Y) && !controllers[index].YButton) {
        controllers[index].YButton = false;
        controllers[index].buttonRelease.emplace("Y");
    }
}

bool GaiaInput::isButtonPressed(int controllerIndex, std::string button)
{
    if (button == "UP") {
        return controllers[controllerIndex].Up;
    }
    // TODO
    else return false;
}

bool GaiaInput::getButtonPress(int controllerIndex, std::string button)
{
    std::transform(button.begin(), button.end(), button.begin(), ::toupper);
    const bool is_in = controllers[controllerIndex].buttonPress.find(button) != controllers[controllerIndex].buttonPress.end();
    return is_in;
}

bool GaiaInput::getButtonRelease(int controllerIndex, std::string button)
{
    std::transform(button.begin(), button.end(), button.begin(), ::toupper);
    const bool is_in = controllers[controllerIndex].buttonRelease.find(button) != controllers[controllerIndex].buttonRelease.end();
    return is_in;
}

/// strength = rumble strength percentage [0.0 , 1.0] | length = duration of effect in miliseconds
void GaiaInput::controllerRumble(int controllerIndex, float strength, int length)
{
    if (strength > 1.0f) strength = 1.0f;
    if (length > 10000) length = 10000;
    if (RumbleHandles[controllerIndex])
    {
        SDL_HapticRumblePlay(RumbleHandles[controllerIndex], strength, length);
    }
}
#pragma region UTILS
void GaiaInput::clearInputs()
{
    keyPress.clear();
    keyRelease.clear();

    if (MOUSE_BUTTON_LEFT.pressed) MOUSE_BUTTON_LEFT.pressed = false;
    else if (MOUSE_BUTTON_LEFT.released)MOUSE_BUTTON_LEFT.released = false;

    if (MOUSE_BUTTON_RIGHT.pressed) MOUSE_BUTTON_RIGHT.pressed = false;
    else if (MOUSE_BUTTON_RIGHT.released)MOUSE_BUTTON_RIGHT.released = false;

    if (MOUSE_BUTTON_MIDDLE.pressed) MOUSE_BUTTON_MIDDLE.pressed = false;
    else if (MOUSE_BUTTON_MIDDLE.released)MOUSE_BUTTON_MIDDLE.released = false;

    for (int i = 0; i < currentControllers; i++) {
        controllers[i].buttonPress.clear();
        controllers[i].buttonRelease.clear();
    }
}
#pragma endregion
