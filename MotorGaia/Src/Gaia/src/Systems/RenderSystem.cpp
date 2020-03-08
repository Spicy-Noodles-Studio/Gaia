#include "RenderSystem.h"

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
	root->renderOneFrame();
}

void RenderSystem::close()
{
	destroy();
}