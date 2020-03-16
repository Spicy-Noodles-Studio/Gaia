#pragma once
#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <set>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"

#include "Singleton.h"
#include "MouseEventListener.h"
#include "KeyboardEventListener.h"
#include "ControllerEventListener.h"

class GAIA_API InputSystem : public Singleton<InputSystem>, public MouseEventListener, public KeyboardEventListener, public ControllerEventListener
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
    int MOUSE_DELTA_X;
    int MOUSE_DELTA_Y;

    struct mouseButton {
        bool pressed = false;
        bool hold = false;
        bool released = false;
    };

    mouseButton MOUSE_BUTTON_LEFT;
    mouseButton MOUSE_BUTTON_RIGHT;
    mouseButton MOUSE_BUTTON_MIDDLE;

    int MOUSE_WHEEL;

    void processMouseInputDown(SDL_MouseButtonEvent& e);
    void processMouseInputUp (SDL_MouseButtonEvent& e);

    // GAMEPAD
    int currentControllers = 0;

    struct gameController {

        SDL_GameController* controller = nullptr;
        SDL_Haptic* controllerRumble = nullptr;
        
        int ID = -1;
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

        int JOYSTICK_DEAD_ZONE = 8000;

        int16_t LeftStickX = 0;
        int16_t LeftStickY = 0;

        int16_t RightStickX = 0;
        int16_t RightStickY = 0;

        uint8_t LeftTrigger = 0;
        uint8_t RightTrigger = 0;

        std::set<std::string> buttonPress; // Stores SDL_KEYDOWN events from current frame
        std::set<std::string> buttonRelease; // Stores SDL_KEYUP events from current frame
    };

    gameController controllers[4];

    void controllerInputDown(int index);
    void controllerInputUp(int index);
    
    // Listener callbacks
    void processKeyDown(std::string keyName, int key);
    void processKeyUp(std::string keyName, int key);
    void processMouseMotion(int x, int y);
    void processMouseLeftButtonDown();
    void processMouseRightButtonDown();
    void processMouseMiddleButtonDown(); 
    void processMouseLeftButtonUp();
    void processMouseRightButtonUp();
    void processMouseMiddleButtonUp();
    void processMouseWheelScrollY(int value);
    void processControllerButtonDown(int index, int button);
    void processControllerButtonUp(int index, int button);
    void processControllerDeviceAdded(int index);
    void processControllerDeviceRemoved(int index);
    void processControllerAxisLeftY(int index, double value);
    void processControllerAxisLeftX(int index, double value);

    // UTILS
    bool flags = true;

    void clearInputs();
    int getFirstFreeController();
    int getControllerByReference(SDL_GameController* handle);
    int getControllerFromEvent(int index);
    int getControllerRemovedIndex(int index);

public:
    InputSystem();
    ~InputSystem();

    void init();
    void close();
    void preUpdate();
    void update();

    void toggleFlags() { flags = !flags; }

    // Keyboard returns
    bool isKeyPressed(SDL_Keycode key) { return keyboardState[key]; };
    bool isKeyPressed(std::string key);
    bool getKeyPress(std::string key);
    bool getKeyRelease(std::string key);

    // Mouse returns
    bool getMouseButtonClick(char button);
    bool getMouseButtonHold(char button);
    bool getMouseButtonRelease(char button);
    /// Returns 1 for wheel_up, -1 for wheel_down, 0 if no movement
    int getMouseWheel() { return MOUSE_WHEEL; }
    std::pair<int, int> getMousePosition() { return std::pair<int, int>(MOUSE_POSITION_X, MOUSE_POSITION_Y); };


    // Controller returns
    bool isButtonPressed(int controllerIndex, std::string button);
    bool getButtonPress(int controllerIndex, std::string button);
    bool getButtonRelease(int controllerIndex, std::string button);

    std::pair<int, int> getLeftJoystick(int controllerIndex) { 
        return std::pair<int, int>(controllers[controllerIndex].LeftStickX, controllers[controllerIndex].LeftStickY); }
    std::pair<int, int> getRightJoystick(int controllerIndex) {
        return std::pair<int, int>(controllers[controllerIndex].RightStickX, controllers[controllerIndex].RightStickY); }

    int getLeftTrigger(int controllerIndex) { return controllers[controllerIndex].LeftTrigger; }
    int getRightTrigger(int controllerIndex) { return controllers[controllerIndex].RightTrigger; }

    void controllerRumble(int controllerIndex, float strength, int length);
    void setDeadZone(int controller, int zone);
};

#endif