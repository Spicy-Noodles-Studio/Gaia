#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <map>
#include <OgreAnimationState.h>

#include "GaiaComponent.h"

namespace Ogre {
	class AnimationStateSet;
	class AnimationState;
}

class GAIA_API Animator : public GaiaComponent
{
private:
	Ogre::AnimationStateSet* animations;

	std::string currentAnimation;
	std::string currentMesh;

public:
	Animator(GameObject* gameObject);
	~Animator();

	void setMesh(const std::string& mesh);

	Ogre::AnimationState* getAnimation(const std::string& animation);

	void playAnimation(const std::string& animation);
	std::string getCurrentAnimation();
	std::vector<std::string> getAllAnimationsNames();
	void printAllAnimationsNames();

	virtual void handleData(ComponentData* data);

	void setLoop(bool loop);
	void setTimePosition(float pos);
	void setLength(float length);

	bool getLoop();
	float getTimePosition();
	float getLength();
};

#endif