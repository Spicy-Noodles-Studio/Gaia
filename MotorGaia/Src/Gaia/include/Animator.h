#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "GaiaComponent.h"
#include "RenderSystem.h"

class Animator : public GaiaComponent
{
private:
	std::map<std::string, Ogre::AnimationState*> animations;
	Ogre::AnimationState* currentAnim;

public:
	Animator(GameObject* gameObject);
	~Animator();
};

#endif