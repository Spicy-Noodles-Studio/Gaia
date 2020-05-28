#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "GaiaComponent.h"
#include <queue>

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

	std::queue<std::string> animationSequence;
	bool endSequenceWithLoop;
public:
	Animator(GameObject* gameObject);
	virtual ~Animator();

	void setMesh(const std::string& mesh);
	void playAnimation(const std::string& animation, bool begin = false);
	void playAnimationSequence(const std::vector<std::string>& sequence, bool endWithLoop = false);
	void updateAnimationSequence();
	const std::string& getCurrentAnimation() const;
	std::vector<std::string> getAllAnimationsNames() const;
	void printAllAnimationsNames() const;

	void setLoop(bool loop);
	void setTimePosition(float pos);
	void setLength(float length);

	bool getLoop() const;
	float getTimePosition() const;
	float getLength() const;
	bool hasEnded() const;
	bool isPlayingSequence() const;
	const std::queue<std::string>& getAnimationSequence() const;

protected:
	virtual void handleData(ComponentData* data);

private:
	Ogre::AnimationState* getAnimation(const std::string& animation) const;
};

#endif