#include "RenderSystem.h"
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include <OgreCompositorManager.h>
#include <OgreCompositor.h>
#include <OgreCompositorInstance.h>

#include "Window.h"
RenderSystem::RenderSystem() : root(nullptr)
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::init(Ogre::Root* root, Window* window_)
{
	this->root = root;
	window = window_;
}

void RenderSystem::render(float deltaTime)
{
	root->renderOneFrame();
}

void RenderSystem::close()
{
	destroy();
}

void RenderSystem::changeParamOfShader(const std::string& material, const std::string& paramName,  float paramValue)
{
	Ogre::GpuProgramParametersSharedPtr x;
	x = Ogre::MaterialManager::getSingleton().getByName(material)->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	x->getSharedParameters().at(0).getSharedParams()->setNamedConstant(paramName, paramValue);
	//	std::cout<<"     "<<x->getConstantDefinition(paramName).isFloat()<<std::endl;
	Ogre::MaterialManager::getSingleton().getByName(material)->getTechnique(0)->getPass(0)->setFragmentProgramParameters(x);
}


void RenderSystem::windowResize(unsigned int width, unsigned int height)
{
	window->resize(width, height);
}

void RenderSystem::setFullscreen(bool fullscreen)
{
	window->setFullscreen(fullscreen);
}

void RenderSystem::ApplyBrightnessToVp(Ogre::Viewport* vp)
{

	Ogre::CompositorManager::getSingleton().addCompositor(vp, "Luminance");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Luminance", true);
}
