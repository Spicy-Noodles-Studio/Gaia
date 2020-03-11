#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(GameObject* gameObject, Ogre::SceneManager* manager) : GaiaComponent(gameObject)
{
	sm = manager;
}

ParticleEmitter::~ParticleEmitter()
{
	delete sm;
	sm = nullptr;
	delete ps;
	ps = nullptr;

}

void ParticleEmitter::newEmitter(Ogre::String source)
{
	ps = sm->createParticleSystem();
	ps->setMaterialName(source);
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
