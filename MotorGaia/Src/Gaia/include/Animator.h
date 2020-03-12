#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <map>
#include <OgreAnimationState.h>

#include "GaiaComponent.h"

class Animator : public GaiaComponent
{
private:
	Ogre::AnimationStateSet* animations;

public:
	Animator(GameObject* gameObject);
	~Animator();

	void setMesh(const std::string& mesh);

	Ogre::AnimationState* getAnimation(const std::string& animation);

	virtual void handleData(ComponentData* data);
};

#endif