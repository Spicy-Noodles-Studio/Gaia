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

void RenderSystem::init(Ogre::Root* root, Window* window)
{
	this->root = root;
	this->window = window;
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
	
	Ogre::MaterialManager::getSingleton().getByName(material)->getTechnique(0)->getPass(0)->setFragmentProgramParameters(x);
}



void RenderSystem::applyBrightness(Ogre::Viewport* vp)
{

	Ogre::CompositorManager::getSingleton().addCompositor(vp, "Luminance");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Luminance", true);
}
