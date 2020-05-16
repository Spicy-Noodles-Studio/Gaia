#include "Cursor.h"
#include <SDL2\SDL.h>
#include "WindowManager.h"
#include "Window.h"
#include <CEGUI\CEGUI.h>
#include "ComponentRegister.h"

REGISTER_FACTORY(Cursor);

Cursor* Cursor::cursor = nullptr;

Cursor::Cursor(GameObject* gameObject) : GaiaComponent(gameObject), visibleOnWindow(true), sprite("")
{
	if (cursor == nullptr) {
		window = WindowManager::GetInstance()->getWindow(0);
	}
	else {
		window = cursor->window;
		visibleOnWindow = cursor->visibleOnWindow;
		sprite = cursor->sprite;
	}
	cursor = this;
}

Cursor::~Cursor()
{

}

void Cursor::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		if (prop.first == "visible") {
			std::stringstream ss(prop.second);
			bool visible = false;
			if (!(ss >> visible)) 
				LOG_ERROR("CURSOR", "Invalid value \"%s\" for property name", prop.second.c_str());
			else
				setVisibleOnWindow(visible);
		}
		else if (prop.first == "sprite") {
			std::stringstream ss(prop.second);
			std::string name;
			if (!(ss >> name))
				LOG_ERROR("CURSOR", "Invalid value \"%s\" for property name", prop.second.c_str());
			else
				setSprite(name);
		}
		else if (prop.first == "spriteVisible") {
			std::stringstream ss(prop.second);
			bool visible = false;
			if (!(ss >> visible))
				LOG_ERROR("CURSOR", "Invalid value \"%s\" for property name", prop.second.c_str());
			else
				setSpriteVisible(visible);
		}
		else {
			LOG_ERROR("CURSOR", "Invalid property name \"%s\"", prop.first.c_str());
		}
	}
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

void Cursor::setSprite(const std::string& name)
{
	sprite = name;
	try {
		auto& context = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
		if (sprite == "")
			context.setDefaultImage(nullptr);
		else
			context.setDefaultImage(name);
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Image named \"%s\" not found", name.c_str());
		sprite = "";
	}
}

void Cursor::setSpriteVisible(bool visible)
{
	try {
		auto& context = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
		context.setVisible(visible);
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Error while setting sprite visibility");
	}
}

bool Cursor::isVisibleOnWindow() const
{
	return visibleOnWindow;
}

bool Cursor::isSpriteVisible() const
{
	try {
		return CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible();
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Error while getting sprite visibility");
	}
	return false;
}
