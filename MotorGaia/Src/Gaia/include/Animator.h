#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <map>

#include <OgreAnimationState.h>

#include "GaiaComponent.h"

class Animator : public GaiaComponent
{
private:
	std::map<std::string, Ogre::AnimationState*> animations;

public:
	Animator(GameObject* gameObject);
	~Animator();

	void setEntity(std::string entity);

	Ogre::AnimationState* getAnimation(std::string animation);
};

#endif