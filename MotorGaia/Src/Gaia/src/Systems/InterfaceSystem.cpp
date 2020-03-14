#include "InterfaceSystem.h"

#include <CEGUI/Event.h>

InterfaceSystem::InterfaceSystem()
{

}

InterfaceSystem::~InterfaceSystem()
{
	
}

void InterfaceSystem::close()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::System::getSingleton().destroy();

	CEGUI::OgreRenderer::destroy(*mRenderer);
	destroy();
}

void InterfaceSystem::setupResources()
{
	CEGUI::DefaultResourceProvider* rp =
		static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	// setup resources
	rp->setResourceGroupDirectory("schemes", "./Assets/UI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "./Assets/UI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "./Assets/UI/fonts/");
	rp->setResourceGroupDirectory("layouts", "./Assets/UI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "./Assets/UI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "./Assets/UI/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "./Assets/UI/xml_schemas/");
	rp->setResourceGroupDirectory("animations", "./Assets/UI/animations/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");

	// load themes
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	// load fonts
	CEGUI::FontManager::getSingleton().createFreeTypeFont("Batang", 12, true, "batang.ttf", "fonts");
}

UIElement* InterfaceSystem::getRoot()
{
	return root;
}

void InterfaceSystem::createRoot()
{
	root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

void InterfaceSystem::init(Window* window)
{
	// init
	mRenderer = &CEGUI::OgreRenderer::create(*window->getRenderWindow());
	CEGUI::System::create(*mRenderer);

	// esto deberia ir en el ResourceManager!!
	//setupResources();

	createRoot();
}

void InterfaceSystem::render()
{
	mRenderer->beginRendering();
	CEGUI::System::getSingleton().renderAllGUIContexts();
	mRenderer->endRendering();
}

void InterfaceSystem::update(float deltaTime)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(deltaTime);
}

UIElement* InterfaceSystem::loadLayout(const std::string& filename)
{
	try {
		return CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
	}
	catch (std::exception e) {
		printf("INTERFACE SYSTEM: trying to load \"%s\" layout\n", filename.c_str());
		return nullptr;
	}
}
