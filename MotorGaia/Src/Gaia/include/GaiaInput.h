#pragma once

#include <iostream>
#include <set>

#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"

#include "Singleton.h"

class GaiaInput : public Singleton<GaiaInput>
{
private:

    int JOYSTICK_DEAD_ZONE = 8000;

    std::set<std::string> keyPress;
    std::set<std::string> keyHold;
    std::set<std::string> keyRelease;

public:
    void init();
    void close();
    void Update();

    void setDeadZone(int zone) { JOYSTICK_DEAD_ZONE = zone; }
};

