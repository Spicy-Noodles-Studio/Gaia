#include "Animator.h"
#include "GameObject.h"
#include "MeshRenderer.h"

Animator::Animator(GameObject* gameObject) : GaiaComponent(gameObject)
{

}

Animator::~Animator()
{

}

void Animator::setEntity(std::string entity)
{
	MeshRenderer* mr = gameObject->getComponent<MeshRenderer>();

	if (mr != nullptr)
		animations = mr->getEntity(entity)->getAllAnimationStates()->getAnimationStates();
}

Ogre::AnimationState* Animator::getAnimation(std::string animation)
{
	return animations[animation];
}