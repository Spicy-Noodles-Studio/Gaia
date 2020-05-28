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
    friend class GaiaCore;
private:  
    // KEYBOARD
    const Uint8* keyboardState; // Stores current keyboard state

    std::set<std::string> keyPress; // Stores SDL_KEYDOWN events from current frame
    std::set<std::string> keyHold; // Stores SDL_KEYDOWN events from current frame
    std::set<std::string> keyRelease; // Stores SDL_KEYUP events from current frame

    // MOUSE
    int MOUSE_POSITION_X;
    int MOUSE_POSITION_Y;

    struct MouseButton {
        bool pressed = false;
        bool hold = false;
        bool released = false;
    };

    MouseButton MOUSE_BUTTON_LEFT;
    MouseButton MOUSE_BUTTON_RIGHT;
    MouseButton MOUSE_BUTTON_MIDDLE;

    int MOUSE_WHEEL;

    void processMouseInputDown(SDL_MouseButtonEvent& e);
    void processMouseInputUp (SDL_MouseButtonEvent& e);

    // GAMEPAD
    int currentControllers = 0;

    struct GameController 
    {
        SDL_GameController* controller = nullptr;
        SDL_Haptic* controllerRumble = nullptr;
        
        int ID = -1;
        int controllerIndex = -1;
        bool isConnected = false;

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

    GameController controllers[4];

    void controllerInputDown(int index);
    void controllerInputUp(int index);
    
    // Listener callbacks
    void processKeyDown(std::string keyName);
    void processKeyUp(std::string keyName);
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

    // UTILS
    void clearInputs();
    int getFirstFreeController() const;
    int getControllerByReference(SDL_GameController* handle) const;
    int getControllerFromEvent(int index) const;
    int getControllerRemovedIndex(int index) const;

    bool mouseUsed;
    bool keyboardUsed;
    bool controllerUsed;

public:
    InputSystem();
    ~InputSystem();

private:
    void init();
    void close();
    void preUpdate();
    void update();
    void postUpdate();

public:
    // Keyboard returns
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyPressed(std::string key) const;
    bool getKeyPress(std::string key) const;
    bool getKeyRelease(std::string key) const;

    // Mouse returns
    bool getMouseButtonClick(char button) const;
    bool getMouseButtonHold(char button) const;
    bool getMouseButtonRelease(char button) const;
    /// Returns 1 for wheel_up, -1 for wheel_down, 0 if no movement
    int getMouseWheel() const;
    std::pair<int, int> getMousePosition() const;

    //Events occurred
    bool isMouseUsed() const;
    bool isKeyboardUsed() const;
    bool isControllerUsed() const;

    // Controller returns
    bool isButtonPressed(int controllerIndex, std::string button) const;
    bool getButtonPress(int controllerIndex, std::string button) const;
    bool getButtonRelease(int controllerIndex, std::string button) const;

    std::pair<int, int> getLeftJoystick(int controllerIndex) const;
    std::pair<int, int> getRightJoystick(int controllerIndex) const;

    int getLeftTrigger(int controllerIndex) const;
    int getRightTrigger(int controllerIndex) const;

    void controllerRumble(int controllerIndex, float strength, int length);
    void setDeadZone(int controller, int zone);

    bool isControllerConnected(int index) const;
};

#endif