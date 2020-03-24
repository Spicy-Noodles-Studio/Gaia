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

namespace CEGUI {
	class Window;
	class OgreRenderer;
	class String;
	class EventArgs;
}
class Window;

typedef std::pair<CEGUI::String, std::function<bool(const CEGUI::EventArgs&)>> UIEvent;


class GAIA_API InterfaceSystem : public Singleton<InterfaceSystem>, public MouseEventListener, public KeyboardEventListener, public WindowEventListener, public ControllerEventListener
{
private:
	CEGUI::OgreRenderer* renderer;
	UIElement* root;

	double deltaX, deltaY;
#ifdef _DEBUG
	UIElement* fpsText;
#endif // _DEBUG

	static std::map<std::string, UIEvent> events;

	void setupResources();
	CEGUI::Key::Scan SDLKeyToCEGUIKey(int key);

	void processControllerButtonDown(int index, int button);
	void processControllerButtonUp(int index, int button);

public:
	InterfaceSystem();
	~InterfaceSystem();

	void init(Window* window);
	void render();
	void update(float deltaTime);
	void close();

	static void registerEvent(const std::string& eventName, UIEvent event);
	static UIEvent getEvent(const std::string& eventName);

	void createRoot();
	UIElement* getRoot();

	UIElement* loadLayout(const std::string& filename);
	void initDefaultResources();
};

#endif