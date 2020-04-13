#pragma once
#ifndef INTERFACE_SYSTEM_H
#define INTERFACE_SYSTEM_H

#include "Singleton.h"
#include "MouseEventListener.h"
#include "KeyboardEventListener.h"
#include "WindowEventListener.h"
#include "ControllerEventListener.h"
#include <CEGUI\CEGUI.h>
#include "UIElement.h"
#include <string>
#include "Window.h"

#include <map>

namespace CEGUI {
	class Window;
	class OgreRenderer;
	class String;
	class EventArgs;
}
class Window;

typedef std::pair<std::string, std::function<bool()>> UIEvent;

class GAIA_API InterfaceSystem : public Singleton<InterfaceSystem>, public MouseEventListener, public KeyboardEventListener, public WindowEventListener, public ControllerEventListener
{
private:
	CEGUI::OgreRenderer* renderer;
	UIElement* root;

	// For controller input
	UIElement* currentLayout;
	std::map<std::string, UIElement*> buttons;
	std::string currentButton;

	void initControllerMenuInput();
	void layoutButtonSearch(UIElement parent);

	double deltaX, deltaY;
#ifdef _DEBUG
	UIElement* fpsText;
#endif // _DEBUG

	static std::map<std::string, UIEvent> events;

	std::map<std::string, CEGUI::String> eventTypes;

	void setupResources();
	CEGUI::Key::Scan SDLKeyToCEGUIKey(int key);

	void processControllerButtonDown(int index, int button);
	void processControllerButtonUp(int index, int button);
	void moveControllerToButton();

	void processMouseMotion(int x, int y);

	void processKeyDown(std::string keyName, int key);
	void processKeyUp(std::string keyName, int key);

public:
	InterfaceSystem();
	~InterfaceSystem();

	void init(Window* window);
	void render();
	void update(float deltaTime);
	void close();

	CEGUI::String getEventType(std::string eventType);

	static void registerEvent(const std::string& eventName, UIEvent event);
	void unregisterEvent(const std::string& eventName);
	static UIEvent getEvent(const std::string& eventName);

	void createRoot();
	UIElement* getRoot();

	UIElement* loadLayout(const std::string& filename);
	void initDefaultResources();

	void clearControllerMenuInput();
};

#endif