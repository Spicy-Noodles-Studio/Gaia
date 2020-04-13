#include "Animator.h"

#include <sstream>
#include <OgreAnimationState.h>
#include <OgreEntity.h>

#include "GameObject.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "ComponentData.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Animator);

Animator::Animator(GameObject* gameObject) : GaiaComponent(gameObject)
{

}

Animator::~Animator()
{

}

void Animator::setMesh(const std::string& mesh)
{
	MeshRenderer* aux = gameObject->getComponent<MeshRenderer>();

	if (aux != nullptr)
	{
		animations = aux->getMesh(mesh)->getAllAnimationStates();
		if(animations != 0)
			gameObject->getScene()->addAnimationSet(gameObject->getName(), animations);
	}
}

Ogre::AnimationState* Animator::getAnimation(const std::string& animation)
{
	return animations->getAnimationState(animation);
}

void Animator::playAnimation(const std::string& animation)
{
	Ogre::AnimationState* prev = getAnimation(currentAnimation);
	prev->setTimePosition(0);
	prev->setEnabled(false);

	getAnimation(animation)->setEnabled(true);
	currentAnimation = animation;
}

std::string Animator::getCurrentAnimation()
{
	return currentAnimation;
}

void Animator::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "anim")
		{
			std::string anim, mesh; 
			if (ss >> anim >> mesh) {
				setMesh(mesh);

				Ogre::AnimationState* aux = getAnimation(anim);
				aux->setEnabled(true);
				currentAnimation = anim;
			}
			else
				LOG("ANIMATOR: wrong value for property %s.\n", prop.first.c_str());
		}
	}
}