#include "ParticleEmitter.h"

#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(ParticleEmitter);

ParticleEmitter::ParticleEmitter(GameObject* gameObject) : GaiaComponent(gameObject), particleSystem(nullptr)
{

}

ParticleEmitter::~ParticleEmitter()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	checkNullAndBreak(particleSystem);

	gameObject->node->detachObject(particleSystem);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);

	sceneManager->destroyParticleSystem(particleSystem);
	particleSystem = nullptr;
}

void ParticleEmitter::newEmitter(const std::string& source)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);

	if (particleSystem != nullptr) {
		checkNullAndBreak(particleSystem);
		gameObject->node->detachObject(particleSystem);
		sceneManager->destroyParticleSystem(particleSystem);
		particleSystem = nullptr;
	}
	try {
		particleSystem = sceneManager->createParticleSystem(gameObject->getName() + "PS", source);
		checkNullAndBreak(particleSystem);
	}
	catch (std::exception exception) {
		LOG_ERROR("PARTICLE EMITTER", "Error ocurred while creating %s", source.c_str());
		return;
	}

	particleSystem->setMaterialName(source);
	gameObject->node->attachObject(particleSystem);
	particleSystem->setEmitting(false);
}

void ParticleEmitter::start()
{
	if (particleSystem != nullptr)
		particleSystem->setEmitting(true);
	else
		LOG_ERROR("PARTICLE EMITTER", "Trying to start a NULL particle emitter");
}

void ParticleEmitter::stop()
{
	if (particleSystem != nullptr)
		particleSystem->setEmitting(false);
	else
		LOG_ERROR("PARTICLE EMITTER", "Trying to stop a NULL particle emitter");
}

void ParticleEmitter::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties()) {
		if (prop.first == "emitter") {
			std::stringstream ss(prop.second);
			std::string source;
			if (ss >> source) {
				newEmitter(source);
			}
			else {
				LOG_ERROR("PARTICLE EMITTER", "Invalid data format. Property \"emitter\"");
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
				LOG_ERROR("PARTICLE EMITTER", "Invalid value \"%s\". Property \"start\"", prop.second.c_str());
			}
		}
		else {
			LOG_ERROR("PARTICLE EMITTER", "Invalid property name \"%s\"", prop.first.c_str());
		}
	}
}
