#include "InterfaceSystem.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
//#include <CEGUI/Renderer.h>

//#include <CEGUI/ImageCodec.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>


InterfaceSystem::InterfaceSystem()
{


}

InterfaceSystem::~InterfaceSystem()
{

}

void InterfaceSystem::setup(Ogre::Root* _root)
{
	root = _root;
	//CEGUI::OgreRenderer* mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	// Init with SDL Window
	CEGUI::OpenGLRenderer& mRenderer = CEGUI::OpenGLRenderer::bootstrapSystem();
	

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");

	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	myRoot = wmgr.createWindow("Vanilla/StaticText", "staticText");
	myRoot->setText("christian tenllado hijo de puta");
	//myRoot->createChild("Generic/ImageButton", "CEGUI/PushButton");

	myRoot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
	myRoot->setSize(CEGUI::USize(CEGUI::UDim(0, 150), CEGUI::UDim(0, 100)));
	//myRoot->setProperty("Image", "WindowsLook/full_image");



	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	/*CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	quit = wmgr.createWindow("Generic/ImageButton", "CEGUI/PushButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));*/
	/*CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");*/

	
}

void InterfaceSystem::render()
{
	 myRoot->render();

}

