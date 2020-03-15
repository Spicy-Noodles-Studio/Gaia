#pragma once

#include <OgreRoot.h>
#include "Singleton.h"
#include "MouseEventListener.h"
#include "KeyboardEventListener.h"
#include "WindowEventListener.h"
#include "ControllerEventListener.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

typedef CEGUI::Window UIElement;

class InterfaceSystem : public Singleton<InterfaceSystem>, public MouseEventListener, public KeyboardEventListener, public WindowEventListener, public ControllerEventListener
{
private:
	CEGUI::OgreRenderer* mRenderer;
	UIElement* root;

	double deltaX, deltaY;

	void setupResources();
	CEGUI::Key::Scan SDLKeyToCEGUIKey(int key);

	void processControllerAxisLeftY(int index, double value);
	void processControllerAxisLeftX(int index, double value);
	void processControllerButtonDown(int index, int button);
	void processControllerButtonUp(int index, int button);

public:
	InterfaceSystem();
	~InterfaceSystem();

	void init(Window* window);
	void render();
	void update(float deltaTime);
	void close();

	void createRoot();
	UIElement* getRoot();

	UIElement* loadLayout(const std::string& filename);

};

