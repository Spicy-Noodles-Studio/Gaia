#include "BrightnessEffect.h"
#include <CEGUI\RendererModules\Ogre\Renderer.h>
#include "WindowManager.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include <OgreGpuProgramParams.h>
#include <OgreTechnique.h>

BrightnessEffect::BrightnessEffect(CEGUI::Window* window) : window(window), brightness(nullptr)
{
	
}

BrightnessEffect::~BrightnessEffect()
{

}

int BrightnessEffect::getPassCount() const
{
	return 1;
}

void BrightnessEffect::performPreRenderFunctions(int pass)
{
	// Texture
	//CEGUI::OgreRenderer* render = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());
	//render->bindShaders();
	if (material == nullptr)
		material = Ogre::MaterialManager::getSingleton().getByName("Brightness").get();
	if (fragmentShader == nullptr || vertexShader == nullptr) {
		auto pass = material->getTechnique(0)->getPass(0);
		vertexShader = pass->getVertexProgram();
		fragmentShader = pass->getFragmentProgram();
	}
	if(root == nullptr)
		root = &Ogre::Root::getSingleton();


	//DEGUb
	if (InputSystem::GetInstance()->getKeyPress("P")) {
		//Texture
		auto x = Ogre::MaterialManager::getSingleton().getByName("Brightness")->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		x->getSharedParameters().at(0).getSharedParams()->setNamedConstant("bright", 0.5f);

		//fragmentShader.get()->setParameter("bright", "0.0");
	}

	//root->getRenderSystem()->bindGpuProgram(vertexShader.get());
	//root->getRenderSystem()->bindGpuProgram(fragmentShader.get());
}

void BrightnessEffect::performPostRenderFunctions()
{
	//root->getRenderSystem()->unbindGpuProgram(Ogre::GpuProgramType::GPT_VERTEX_PROGRAM);
	//root->getRenderSystem()->unbindGpuProgram(Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
}

bool BrightnessEffect::realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry)
{
	return true;
}

bool BrightnessEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
	return true;
}
