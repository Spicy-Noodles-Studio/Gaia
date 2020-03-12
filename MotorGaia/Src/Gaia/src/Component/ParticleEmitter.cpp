#include "ParticleEmitter.h"
#include "Scene.h"
#include <OgreSceneNode.h>

ParticleEmitter::ParticleEmitter(GameObject* gameObject) : GaiaComponent(gameObject)
{
	sm = gameObject->getScene()->getSceneManager();
	go = gameObject;
}

ParticleEmitter::~ParticleEmitter()
{
	delete sm;
	sm = nullptr;
	delete ps;
	ps = nullptr;
	delete go;
	go = nullptr;

}

void ParticleEmitter::newEmitter(std::string name,std::string source)
{
	ps = sm->createParticleSystem(name,source);
	ps->setMaterialName(source);
	go->node->attachObject(ps);
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
