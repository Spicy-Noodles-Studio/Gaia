#include "Animator.h"

#include <sstream>
#include <OgreAnimationState.h>
#include <OgreEntity.h>

#include "GameObject.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Animator);

Animator::Animator(GameObject* gameObject) : GaiaComponent(gameObject), endSequenceWithLoop(false), currentAnimation(""), animations(nullptr)
{

}

Animator::~Animator()
{

}

void Animator::setMesh(const std::string& mesh)
{
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
	checkNullAndBreak(meshRenderer);

	if (currentAnimation != "") 
	{
		Ogre::AnimationState* previous = getAnimation(currentAnimation);
		checkNullAndBreak(previous);
		previous->setTimePosition(0);
		previous->setEnabled(false);
		currentAnimation = "";
	} 
	// clear previous sequence
	std::queue<std::string>().swap(animationSequence);

	animations = meshRenderer->getMesh(mesh)->getAllAnimationStates();
	if (animations != nullptr) {
		checkNullAndBreak(gameObject);
		Scene* scene = gameObject->getScene();
		checkNullAndBreak(scene);
		scene->addAnimationSet(gameObject->getName(), animations);
	}
	currentMesh = mesh;
}

Ogre::AnimationState* Animator::getAnimation(const std::string& animation) const
{
	checkNullAndBreak(animations, nullptr);
	Ogre::AnimationState* animationState = animations->getAnimationState(animation);
	
	checkNullAndBreak(animationState, nullptr);
	return animationState;
}

void Animator::playAnimation(const std::string& animation, bool begin)
{
	if (currentAnimation == animation && !begin) return;

	if (currentAnimation != "")
	{
		Ogre::AnimationState* previous = getAnimation(currentAnimation);
		checkNullAndBreak(previous);
		previous->setTimePosition(0);
		previous->setEnabled(false);
	}
	Ogre::AnimationState* animationState = getAnimation(animation);
	checkNullAndBreak(animationState);
	animationState->setEnabled(true);
	currentAnimation = animation;
}

void Animator::playAnimationSequence(const std::vector<std::string>& sequence, bool endWithLoop)
{
	// clear previous sequence
	std::queue<std::string>().swap(animationSequence);

	// play first animation
	if (!sequence.size()) return;
	playAnimation(sequence[0]);
	setLoop(false);

	// add the rest to the queue
	for (int i = 1; i < sequence.size(); i++)
		animationSequence.push(sequence[i]);

	endSequenceWithLoop = endWithLoop;
}

void Animator::updateAnimationSequence()
{
	if (!animationSequence.empty() && hasEnded())
	{
		playAnimation(animationSequence.front()); animationSequence.pop();

		if (animationSequence.empty() && endSequenceWithLoop)
			setLoop(true);
		else
			setLoop(false);
	}
}

const std::string& Animator::getCurrentAnimation() const
{
	return currentAnimation;
}

std::vector<std::string> Animator::getAllAnimationsNames() const
{
	std::vector<std::string> names;
	checkNullAndBreak(animations, names);

	for (auto anim : animations->getAnimationStateIterator())
		names.push_back(anim.first);

	return names;
}

void Animator::printAllAnimationsNames() const
{
	LOG("%s MESH ANIMATIONS: ", currentMesh.c_str());
	for (auto anim : animations->getAnimationStateIterator())
		LOG(" - %s", anim.first.c_str());
	LOG("");
}

void Animator::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);
		if (prop.first == "anim")
		{
			std::string animationName, mesh; 
			if (ss >> animationName >> mesh) {
				setMesh(mesh);

				Ogre::AnimationState* animation = getAnimation(animationName);
				checkNull(animation);

				if (animation == nullptr) continue;
				animation->setEnabled(true);
				currentAnimation = animationName;
			}
			else
				LOG_ERROR("ANIMATOR", "Wrong value for property %s.", prop.first.c_str());
		}
	}
}

void Animator::setLoop(bool loop)
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation);
	animation->setLoop(loop);
}

void Animator::setTimePosition(float pos)
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation);
	animation->setTimePosition(Ogre::Real(pos));
}

void Animator::setLength(float length)
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation);
	animation->setLength(Ogre::Real(length));
}

bool Animator::getLoop() const
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation, false);
	return animation->getLoop();
}

float Animator::getTimePosition() const
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation, 0.0f);
	return float(animation->getTimePosition());
}

float Animator::getLength() const
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation, 0.0f);
	return float(animation->getLength());
}

bool Animator::hasEnded() const
{
	Ogre::AnimationState* animation = getAnimation(currentAnimation);
	checkNullAndBreak(animation, false);
	return animation->hasEnded();
}

bool Animator::isPlayingSequence() const
{
	return !animationSequence.empty();
}

const std::queue<std::string>& Animator::getAnimationSequence() const
{
	return animationSequence;
}
