#include "InterfaceSystem.h"

//#include <CEGUI/RendererModules/Ogre/Renderer.h>
//#include <CEGUI/Renderer.h>
//#include <CEGUI/CEGUI.h>
//#include <CEGUI/ImageCodec.h>
//#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

#include <CEGUI/Event.h>

InterfaceSystem::InterfaceSystem()
{


}

InterfaceSystem::~InterfaceSystem()
{

}

bool InterfaceSystem::clicked(const CEGUI::EventArgs& args)
{
	printf("jumpedd");
	return false;
}

void InterfaceSystem::setup(Ogre::Root* _root, Window* window)
{
	root = _root;
	
	// init
	mRenderer = &CEGUI::OgreRenderer::create(*window->mWindow);
	CEGUI::System::create(*mRenderer);

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

	// load theme
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	// load font
	CEGUI::FontManager::getSingleton().createFreeTypeFont("Batang", 12, true, "batang.ttf", "fonts");

	// test panel with text
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	myRoot = wmgr.createWindow("TaharezLook/StaticText", "staticText");
	myRoot->setFont("Batang");
	myRoot->setText("christian tenllado hijo de puta");

	myRoot->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	myRoot->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	// test button
	CEGUI::Window* button = NULL;
	button = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "JumpPushButton");  // Create Window
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(button);
	button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
	button->setText("Jump!");
	button->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::Event::Subscriber(&InterfaceSystem::clicked,this));
	

	
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

