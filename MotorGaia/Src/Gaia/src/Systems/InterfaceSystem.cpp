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

	//mRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*window->mWindow);
	//SDL_ShowCursor(SDL_DISABLE);

	//mRenderer = &CEGUI::OpenGLRenderer::bootstrapSystem();

	// create renderer and enable extra states
	mRenderer = &CEGUI::OgreRenderer::create(*window->mWindow);
	//mRenderer->enableExtraStateSettings(true);

	// create CEGUI system object
	CEGUI::System::create(*mRenderer);

	//CEGUI::OpenGLRenderer* renderer = new CEGUI::OpenGLRenderer(CEGUI::OpenGLRenderer::);

	//CEGUI::System::getSingleton().renderAllGUIContexts();
	//CEGUI::System::getSingleton().create(mRenderer);
	//mRenderer.setActiveRenderTarget()

	//CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root->getwi)

	CEGUI::DefaultResourceProvider* rp =
		static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../Dependencies/cegui-0.8.7/datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../Dependencies/cegui-0.8.7/datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../Dependencies/cegui-0.8.7/datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../Dependencies/cegui-0.8.7/datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../Dependencies/cegui-0.8.7/datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../Dependencies/cegui-0.8.7/datafiles/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "../Dependencies/cegui-0.8.7/datafiles/xml_schemas/");
	rp->setResourceGroupDirectory("animations", "../Dependencies/cegui-0.8.7/datafiles/animations/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");

	//CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");

	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	//CEGUI::System::getSingleton().mouse

	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	myRoot = wmgr.createWindow("TaharezLook/StaticText", "staticText");
	myRoot->setText("christian tenllado hijo de puta");

	myRoot->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	myRoot->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
	//myRoot->setProperty("Image", "WindowsLook/full_image");

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
	//CEGUI::System::getSingleton().event
	mRenderer->endRendering();
}

