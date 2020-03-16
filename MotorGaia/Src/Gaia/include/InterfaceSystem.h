#pragma once

#include "Singleton.h"
#include <string>

namespace CEGUI {
	class Window;
	class OgreRenderer;
}
typedef CEGUI::Window UIElement;

class Window;

class GAIA_API InterfaceSystem : public Singleton<InterfaceSystem>
{
private:
	CEGUI::OgreRenderer* mRenderer;
	UIElement* root;

	void setupResources();

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

