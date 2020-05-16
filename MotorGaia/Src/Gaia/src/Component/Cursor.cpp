#include "Cursor.h"
#include <SDL2\SDL.h>
#include "WindowManager.h"
#include "Window.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Cursor);

Cursor::Cursor(GameObject* gameObject) : GaiaComponent(gameObject), visibleOnWindow(true)
{
	window = WindowManager::GetInstance()->getWindow(0);
}

Cursor::~Cursor()
{

}

void Cursor::handleData(ComponentData* data)
{
	visibleOnWindow = true;

	for (auto prop : data->getProperties()) {
		if (prop.first == "visibleOnWindow") {
			std::stringstream ss(prop.second);
			bool visible = false;
			if (!(ss >> visible)) 
				LOG_ERROR("CURSOR", "Invalid value \"%s\" for property name", prop.second.c_str());
		}
		else {
			LOG_ERROR("CURSOR", "Invalid property name \"%s\"", prop.first.c_str());
		}
	}

	setVisibleOnWindow(visibleOnWindow);
}

void Cursor::getPosition(int* x, int* y)
{
	SDL_GetMouseState(x, y);
}

void Cursor::setPosition(int x, int y)
{
	SDL_WarpMouseInWindow(window->getSDLWindow(), x, y);
}

void Cursor::setVisibleOnWindow(bool visible)
{
	visibleOnWindow = visible;
	if(SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE) < 0)
		LOG_ERROR("CURSOR","Error trying to change cursor visibility");
}