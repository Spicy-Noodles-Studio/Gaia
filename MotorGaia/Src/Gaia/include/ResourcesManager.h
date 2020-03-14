#pragma once

#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <thread>
#include <map>
#include "DataLoader.h"
#include "SceneData.h"
#include "GameObjectData.h"

#include <OgreFileSystemLayer.h>
#include "ShaderTechniqueResolver.h"
#include <SoundSystem.h>

class ResourcesManager
{
public:
	ResourcesManager(const std::string& filePath);
	~ResourcesManager();

	void init();
	void close();

	bool registerSceneData(SceneData* data);
	bool registerBlueprint(GameObjectData* data);

	static const SceneData* getSceneData(const std::string& name);
	static const SceneData* getSceneData(int index);
	static const GameObjectData* getBlueprint(const std::string& name);
	static Sound* getSound(const std::string& name);

private:
	void loadResources(const std::string& resourceType, const std::string& filename);
	
	void loadScenes(const std::string& filename);
	void loadBlueprints(const std::string& filename);
	void loadSounds(const std::string& filename);
	void loadOgreResources(const std::string& filename);
	void loadInterfaceResources(const std::string& filename);

	void loadScene(const std::string& filename);
	void loadBlueprint(const std::string& filename);
	void loadSound(const std::string& filename);

	bool initShaderSystem();
	void destroyShaderSystem();

private:
	std::mutex sceneDataMutex;
	std::mutex blueprintMutex;
	std::mutex soundMutex;
	std::mutex interfaceMutex;

	std::string resourcesPath;
	DataLoader dataLoader;

	static std::map<std::string, SceneData*> sceneData;
	static std::map<std::string, GameObjectData*> blueprintData;
	static std::map<std::string, Sound*> sounds;

	Ogre::FileSystemLayer* fileSystemLayer;
	std::string shaderLibPath;
	Ogre::RTShader::ShaderGenerator* shaderGenerator; // The Shader generator instance.
	ShaderTechniqueResolver* shaderTechniqueResolver; // Shader generator material manager listener.

};

#endif
