#include "RenderSystem.h"
#include "ErrorManagement.h"
#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include <OgreCompositorManager.h>
#include <OgreCompositor.h>
#include <OgreCompositorInstance.h>


RenderSystem::RenderSystem() : root(nullptr)
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::init(Ogre::Root* root)
{
	this->root = root;
}

void RenderSystem::render(float deltaTime)
{
	checkNullAndBreak(root);

	root->renderOneFrame(deltaTime);
}

void RenderSystem::close()
{
	destroy();
}

void RenderSystem::changeParamOfShader(const std::string& material, const std::string& paramName, float paramValue)
{
	Ogre::GpuProgramParametersSharedPtr programParameters;
	Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
	checkNullAndBreak(materialManager);
	Ogre::Material* mMaterial = materialManager->getByName(material).get();
	checkNullAndBreak(mMaterial);
	Ogre::Technique* technique = mMaterial->getTechnique(0);
	checkNullAndBreak(technique);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass);
	programParameters = pass->getFragmentProgramParameters();
	checkNullAndBreak(programParameters);

	auto& sharedParameters = programParameters->getSharedParameters();
	if (!sharedParameters.size()) return;

	Ogre::GpuSharedParameters* shared = sharedParameters[0].getSharedParams().get();
	checkNullAndBreak(shared);
	shared->setNamedConstant(paramName, paramValue);
	
	pass->setFragmentProgramParameters(programParameters);
}


void RenderSystem::applyBrightness(Ogre::Viewport* viewport)
{
	checkNullAndBreak(viewport);
	Ogre::CompositorManager* compositorManager = Ogre::CompositorManager::getSingletonPtr();
	checkNullAndBreak(compositorManager);

	compositorManager->addCompositor(viewport, "Brightness");
	compositorManager->setCompositorEnabled(viewport, "Brightness", true);
}
