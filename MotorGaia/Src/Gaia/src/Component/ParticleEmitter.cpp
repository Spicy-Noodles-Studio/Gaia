#include "ParticleEmitter.h"
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "GameObject.h"
#include "Scene.h"

ParticleEmitter::ParticleEmitter(GameObject* gameObject) : GaiaComponent(gameObject), particleSystem(nullptr)
{

}

ParticleEmitter::~ParticleEmitter()
{
	if (particleSystem != nullptr) {
		gameObject->node->detachObject(particleSystem);
		gameObject->getScene()->getSceneManager()->destroyParticleSystem(particleSystem);
		particleSystem = nullptr;
	}
}

void ParticleEmitter::newEmitter(const std::string& source)
{
	if (particleSystem != nullptr) {
		gameObject->node->detachObject(particleSystem);
		gameObject->getScene()->getSceneManager()->destroyParticleSystem(particleSystem);
		particleSystem = nullptr;
	}

	particleSystem = gameObject->getScene()->getSceneManager()->createParticleSystem(gameObject->getName() + "PS", source);
	particleSystem->setMaterialName(source);
	gameObject->node->attachObject(particleSystem);
	particleSystem->setEmitting(false);
}

void ParticleEmitter::start()
{
	if (particleSystem != nullptr)
		particleSystem->setEmitting(true);
	else
		LOG("PARTICLE EMITTER: trying to start a NULL particle emitter\n");
}

void ParticleEmitter::stop()
{
	if (particleSystem != nullptr)
		particleSystem->setEmitting(false);
	else
		LOG("PARTICLE EMITTER: trying to stop a NULL particle emitter\n");
}

void ParticleEmitter::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		if (prop.first == "emitter") {
			std::stringstream ss(prop.second);
			std::string source;
			if (ss >> source) {
				newEmitter(source);
			}
			else {
				LOG("PARTICLE EMITTER: invalid data format. Property \"emitter\"\n");
			}
		}
		else if (prop.first == "start") {
			if (prop.second == "true") {
				start();
			}
			else if (prop.second == "false") {
				stop();
			}
			else {
				LOG("PARTICLE EMITTER: invalid value \"%s\". Property \"start\"\n", prop.second.c_str());
			}
		}
		else {
			LOG("PARTICLE EMITTER: invalid property name \"%s\"\n", prop.first.c_str());
		}
	}
}
