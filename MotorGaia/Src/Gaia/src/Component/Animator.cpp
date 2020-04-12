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

Animator::Animator(GameObject* gameObject) : GaiaComponent(gameObject), endSequenceWithLoop(false)
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
		currentMesh = mesh;
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

void Animator::playAnimationSequence(const std::vector<std::string>& sequence, bool endWithLoop)
{
	// clear previous sequence
	while (!animSequence.empty()) animSequence.pop();

	// play first animation
	playAnimation(sequence[0]);

	// add the rest to the queue
	for (int i = 1; i < sequence.size(); i++)
		animSequence.push(sequence[i]);

	endSequenceWithLoop = endWithLoop;
}

void Animator::updateAnimationSequence()
{
	if (!animSequence.empty() && hasEnded())
	{
		playAnimation(animSequence.front()); animSequence.pop();

		if (animSequence.empty() && endSequenceWithLoop)
			setLoop(true);
		else
			setLoop(false);
	}
}

std::string Animator::getCurrentAnimation()
{
	return currentAnimation;
}

std::vector<std::string> Animator::getAllAnimationsNames()
{
	std::vector<std::string> names;

	for (auto anim : animations->getAnimationStateIterator())
		names.push_back(anim.first);

	return names;
}

void Animator::printAllAnimationsNames()
{
	printf("%s MESH ANIMATIONS: \n");
	for (auto anim : animations->getAnimationStateIterator())
		printf(" - %s\n", anim.first.c_str());
	printf("\n");
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

void Animator::setLoop(bool loop)
{
	auto anim = getAnimation(currentAnimation);
	anim->setLoop(loop);
}

void Animator::setTimePosition(float pos)
{
	auto anim = getAnimation(currentAnimation);
	anim->setTimePosition(Ogre::Real(pos));
}

void Animator::setLength(float length)
{
	auto anim = getAnimation(currentAnimation);
	anim->setLength(Ogre::Real(length));
}

bool Animator::getLoop()
{
	return getAnimation(currentAnimation)->getLoop();
}

float Animator::getTimePosition()
{
	return float(getAnimation(currentAnimation)->getTimePosition());
}

float Animator::getLength()
{
	return float(getAnimation(currentAnimation)->getLength());
}

bool Animator::hasEnded()
{
	return getAnimation(currentAnimation)->hasEnded();
}

bool Animator::isPlayingSequence() const
{
	return !animSequence.empty();
}
