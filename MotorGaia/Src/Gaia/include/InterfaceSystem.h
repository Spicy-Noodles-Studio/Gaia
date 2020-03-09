#pragma once

#include <OgreRoot.h>
#include "Singleton.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

typedef CEGUI::Window UIElement;

class InterfaceSystem : public Singleton<InterfaceSystem>
{
private:
	CEGUI::OgreRenderer* mRenderer;
	UIElement* root;

	void setupResources();

	bool clicked(const CEGUI::EventArgs& args);

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

