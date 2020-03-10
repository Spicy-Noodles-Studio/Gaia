#pragma once
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <Ogre.h>

#include "GaiaComponent.h"
#include <OgreParticleSystem.h>
#include<OgreSceneManager.h>
#include <RenderSystem.h>



class ParticleEmitter : public GaiaComponent, public RenderSystem
{
private:
	Ogre::ParticleSystem* ps = nullptr;
	Ogre::SceneManager* sm = nullptr;

public:
	ParticleEmitter(GameObject* gameObject);
	~ParticleEmitter();
	void newEmitter(Ogre::String name, Ogre::String source);
	void start();
	void stop();
};

#endif