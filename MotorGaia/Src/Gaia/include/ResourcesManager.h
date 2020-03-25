#pragma once

#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <thread>
#include <map>
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
	bool registerBlueprint(BlueprintData* data);

	static const SceneData* getSceneData(const std::string& name);
	static const SceneData* getSceneData(int index);
	static const BlueprintData* getBlueprint(const std::string& name);
	static Sound* getSound(const std::string& name);

private:	
	void locateResourceType(const std::string& resourceType, const std::string& filename);
	void locateScenes(const std::string& filename);
	void locateBlueprints(const std::string& filename);
	void locateSounds(const std::string& filename);
	void locateOgreResources(const std::string& filename);

	void locateScene(const std::string& filename);
	void locateBlueprint(const std::string& filename);
	void loadSound(const std::string& filename);

	void initializeAllResources();
	void initializeOgreResources();
	void initializeScenes();
	void initializeBlueprints();
	void initializeSounds();

	bool initShaderSystem();
	void destroyShaderSystem();

public:
	std::mutex sceneDataMutex;
	std::mutex blueprintMutex;
	std::mutex soundMutex;

	std::string resourcesPath;

	static std::map<std::string, SceneData*> sceneData;
	static std::map<std::string, BlueprintData*> blueprintData;
	static std::map<std::string, Sound*> sounds;

	Ogre::FileSystemLayer* fileSystemLayer;
	std::string shaderLibPath;
	Ogre::RTShader::ShaderGenerator* shaderGenerator; // The Shader generator instance.
	ShaderTechniqueResolver* shaderTechniqueResolver; // Shader generator material manager listener.

};

#endif
