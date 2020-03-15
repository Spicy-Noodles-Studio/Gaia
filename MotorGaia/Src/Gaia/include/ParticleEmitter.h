#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "GaiaComponent.h"
#include <OgreParticleSystem.h>


class GAIA_API ParticleEmitter : public GaiaComponent
{
private:
	Ogre::ParticleSystem* particleSystem = nullptr;

public:
	ParticleEmitter(GameObject* gameObject);
	~ParticleEmitter();
	void newEmitter(const std::string& source);
	void start();
	void stop();

	virtual void handleData(ComponentData* data);
};

#endif