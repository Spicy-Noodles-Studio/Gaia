#include "GaiaInput.h"

void GaiaInput::init()
{

}

void GaiaInput::close()
{
	//CERRAR SDL TETES
	SDL_Quit();
}

void GaiaInput::Update()
{
    // LIMPIAR PRESS
    // COMPROBAR RELEASE PARA LIMPIAR HOLD
    // LIMPIAR HOLD

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            keyPress.emplace(SDL_GetKeyName(event.key.keysym.sym));
            break;
        case SDL_KEYUP:
            keyRelease.emplace(SDL_GetKeyName(event.key.keysym.sym));
            break;
        case SDL_MOUSEMOTION:
            std::cout << "Mouse Motion\n";
            break;
        case SDL_QUIT:
            break;
        default:
            std::cout << "I don't know what this event is!\n";
        }
    }
}
