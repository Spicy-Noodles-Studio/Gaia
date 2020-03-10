#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter(GameObject* gameObject) : GaiaComponent(gameObject)
{
	sm = getSceneManager();
}

ParticleEmitter::~ParticleEmitter()
{
	delete sm;
	sm = nullptr;
	delete ps;
	ps = nullptr;

}

void ParticleEmitter::newEmitter(Ogre::String name, Ogre::String source)
{
	ps = sm->createParticleSystem(name,source);
	ps->setEmitting(false);
}

void ParticleEmitter::start()
{
	ps->setEmitting(true);
}

void ParticleEmitter::stop()
{
	ps->setEmitting(false);
}
