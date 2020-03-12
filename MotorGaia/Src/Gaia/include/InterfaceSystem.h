#pragma once

#include <OgreRoot.h>
#include "Singleton.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

typedef CEGUI::Window UIElement;
typedef std::pair<CEGUI::String, std::function<bool(const CEGUI::EventArgs&)>> UIEvent;

class InterfaceSystem : public Singleton<InterfaceSystem>
{
private:
	CEGUI::OgreRenderer* mRenderer;
	UIElement* root;

#ifdef _DEBUG
	UIElement* fpsText;
#endif // _DEBUG

	static std::map<std::string, UIEvent> events;

	void setupResources();

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

};

