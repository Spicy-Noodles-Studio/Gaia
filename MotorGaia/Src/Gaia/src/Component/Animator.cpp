#include "Animator.h"
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
		animations = aux->getMesh(mesh)->getAllAnimationStates()->getAnimationStates();
}

void Animator::storeInScene()
{
	for (auto anim : animations)
		gameObject->getScene()->getAnimations()[anim.first + " " + gameObject->getName()] = anim.second;
}

Ogre::AnimationState* Animator::getAnimation(const std::string& animation)
{
	return animations[animation];
}

void Animator::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		Ogre::AnimationState* aux = nullptr;

		std::stringstream ss(prop.second);

		if (prop.first == "anim")
		{
			std::string anim, mesh; ss >> anim >> mesh;
			setMesh(mesh);
			storeInScene();

			aux = getAnimation(anim);
			aux->setEnabled(true);
		}
		else if (prop.first == "loop")
			aux->setLoop(true);
	}
}