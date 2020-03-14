#pragma once

#include <OgreRoot.h>
#include "Singleton.h"
#include "MouseEventListener.h"
#include "KeyboardEventListener.h"
#include "WindowEventListener.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

typedef CEGUI::Window UIElement;

class InterfaceSystem : public Singleton<InterfaceSystem>, public MouseEventListener, public KeyboardEventListener, public WindowEventListener
{
private:
	CEGUI::OgreRenderer* mRenderer;
	UIElement* root;

	void setupResources();
	CEGUI::Key::Scan SDLKeyToCEGUIKey(int key);

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

