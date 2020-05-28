#include "Cursor.h"
#include <SDL2\SDL.h>
#include "WindowManager.h"
#include "Window.h"
#include <CEGUI\CEGUI.h>
#include "ComponentRegister.h"

REGISTER_FACTORY(Cursor);

Cursor::Cursor(GameObject* gameObject) : GaiaComponent(gameObject), window(nullptr), visibleOnWindow(true), sprite("")
{
	WindowManager* windowManager = WindowManager::GetInstance();
	checkNullAndBreak(windowManager);

	window = windowManager->getWindow(0);
	checkNull(window);

	auto& context = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
	const CEGUI::Image* defaultImage = context.getDefaultImage();

	checkNullAndBreak(defaultImage);
	sprite = defaultImage->getName().c_str();

	setVisibleOnWindow(true);
	setSpriteVisible(true);
}

Cursor::~Cursor()
{

}

void Cursor::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

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
	try {
		SDL_GetMouseState(x, y);
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Something went wrong when trying to get position");
	}
}

void Cursor::setPosition(int x, int y)
{
	checkNullAndBreak(window);
	try {
		SDL_WarpMouseInWindow(window->getSDLWindow(), x, y);
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Something went wrong when trying to set position");
	}
}

void Cursor::setVisibleOnWindow(bool visible)
{
	try {
		SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
		visibleOnWindow = visible;
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Error trying to change cursor visibility");
	}
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
		bool visible = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible();
		return visible;
	}
	catch (std::exception exception) {
		LOG_ERROR("CURSOR", "Error while getting sprite visibility");
	}
	return false;
}
