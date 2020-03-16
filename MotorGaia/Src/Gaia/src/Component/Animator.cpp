#include "Animator.h"
#include <OgreAnimationState.h>
#include <OgreEntity.h>
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "ComponentData.h"
#include <sstream>

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
		gameObject->getScene()->addAnimationSet(gameObject->getName(), animations);
	}
}

Ogre::AnimationState* Animator::getAnimation(const std::string& animation)
{
	return animations->getAnimationState(animation);
}

void Animator::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "anim")
		{
			std::string anim, mesh; ss >> anim >> mesh;
			setMesh(mesh);

			Ogre::AnimationState* aux = getAnimation(anim);
			aux->setEnabled(true);
		}
	}
}