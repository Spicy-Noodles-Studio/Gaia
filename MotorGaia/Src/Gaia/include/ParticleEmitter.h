#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "GaiaComponent.h"

namespace Ogre {
	class ParticleSystem;
}

class GAIA_API ParticleEmitter : public GaiaComponent
{
private:
	Ogre::ParticleSystem* particleSystem;

public:
	ParticleEmitter(GameObject* gameObject);
	virtual ~ParticleEmitter();

	void newEmitter(const std::string& source);
	void start();
	void stop();

protected:
	virtual void handleData(ComponentData* data);
};

#endif