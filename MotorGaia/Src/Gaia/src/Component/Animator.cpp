#include "Animator.h"
#include "GameObject.h"
#include "MeshRenderer.h"

Animator::Animator(GameObject* gameObject) : GaiaComponent(gameObject)
{

}

Animator::~Animator()
{

}

void Animator::setMesh(const std::string& mesh)
{
	MeshRenderer* mr = gameObject->getComponent<MeshRenderer>();

	if (mr != nullptr)
		animations = mr->getMesh(mesh)->getAllAnimationStates()->getAnimationStates();
}

Ogre::AnimationState* Animator::getAnimation(const std::string& animation)
{
	return animations[animation];
}

void Animator::handleData(ComponentData* data)
{

}