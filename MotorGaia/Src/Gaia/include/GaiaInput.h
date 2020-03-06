#pragma once

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <set>

#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"

#include "Singleton.h"

class GaiaInput : public Singleton<GaiaInput>
{
#define MAX_CONTROLLERS 4

private:

    

    // KEYBOARD
    const Uint8* keyboardState; // Stores current keyboard state

    std::set<std::string> keyPress; // Stores SDL_KEYDOWN events from current frame
    std::set<std::string> keyHold; // Stores SDL_KEYDOWN events from current frame
    std::set<std::string> keyRelease; // Stores SDL_KEYUP events from current frame

    // MOUSE
    int MOUSE_POSITION_X;
    int MOUSE_POSITION_Y;

    struct mouseButton {
        bool pressed = false;
        bool hold = false;
        bool released = false;
    };

    mouseButton MOUSE_BUTTON_LEFT;
    mouseButton MOUSE_BUTTON_RIGHT;
    mouseButton MOUSE_BUTTON_MIDDLE;

    void processMouseInputDown(SDL_MouseButtonEvent& e);
    void processMouseInputUp (SDL_MouseButtonEvent& e);

    // GAMEPAD
    int JOYSTICK_DEAD_ZONE = 8000;
    int currentControllers = 0;

    struct gameController {

        SDL_GameController* controller = nullptr;
        SDL_Haptic* controllerRumble = nullptr;
        int controllerIndex = -1;
        bool isConected = false;

        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;
        bool Start = false;
        bool Back = false;
        bool LeftShoulder = false;
        bool RightShoulder = false;
        bool AButton = false;
        bool BButton = false;
        bool XButton = false;
        bool YButton = false;

        int16_t LeftStickX = 0;
        int16_t LeftStickY = 0;

        int16_t RightStickX = 0;
        int16_t RightStickY = 0;

        uint8_t LeftTrigger = 0;
        uint8_t RightTrigger = 0;

        std::set<std::string> buttonPress; // Stores SDL_KEYDOWN events from current frame
        std::set<std::string> buttonRelease; // Stores SDL_KEYUP events from current frame
    };

    SDL_GameController* ControllerHandles[MAX_CONTROLLERS]; // Array of controller handles
    SDL_Haptic* RumbleHandles[MAX_CONTROLLERS]; // Array of haptic devices (rumble)
    gameController controllers[4];

    void controllerInputDown(int index);
    void controllerInputUp(int index);
    
    


    // UTILS
    void clearInputs();
    int getFirstFreeController();
    int getControllerByReference(SDL_GameController* handle);

public:

    // está feo...
    bool exit = false;

    void init();
    void close();
    void update();

    void setDeadZone(int zone) { JOYSTICK_DEAD_ZONE = zone; }

    // Keyboard returns
    bool isKeyPressed(SDL_Keycode key) { return keyboardState[key]; };
    bool isKeyPressed(std::string key);
    bool getKeyPress(std::string key);
    bool getKeyRelease(std::string key);

    // Mouse returns
    bool getMouseButtonClick(char button);
    bool getMouseButtonHold(char button);
    bool getMouseButtonRelease(char button);
    std::pair<int, int> getMousePosition() { return std::pair<int, int>(MOUSE_POSITION_X, MOUSE_POSITION_Y); };

    // Controller returns
    bool isButtonPressed(int controllerIndex, std::string button);
    bool getButtonPress(int controllerIndex, std::string button);
    bool getButtonRelease(int controllerIndex, std::string button);

    std::pair<int, int> getLeftJoystick(int controllerIndex) { 
        return std::pair<int, int>(controllers[controllerIndex].LeftStickX, controllers[controllerIndex].LeftStickY); }
    std::pair<int, int> getRightJoystick(int controllerIndex) {
        return std::pair<int, int>(controllers[controllerIndex].RightStickX, controllers[controllerIndex].RightStickY); }

    int getLeftTrigger(int controllerIndex) { controllers[controllerIndex].LeftTrigger; }
    int getRightTrigger(int controllerIndex) { controllers[controllerIndex].RightTrigger; }

    void controllerRumble(int controllerIndex, float strength, int length);
};

