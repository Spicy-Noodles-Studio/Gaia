#pragma once

#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <map>
#include "DataLoader.h"
#include "SceneData.h"
#include "GameObjectData.h"

#include <OgreFileSystemLayer.h>
#include <OgreSGTechniqueResolverListener.h>


class ResourcesManager
{
public:
	ResourcesManager(const std::string& filePath);
	~ResourcesManager();

	void init();
	void clean();

	void registerSceneData(SceneData* data);
	void registerBlueprint(GameObjectData* data);

	static const SceneData* getSceneData(const std::string& name);
	static const GameObjectData* getBlueprint(const std::string& name);

private:
	void loadResources(const std::string& resourceType, const std::string& filename);
	
	void loadScenes(const std::string& filename);
	void loadBlueprints(const std::string& filename);
	void loadOgreResources(const std::string& filename);

	void loadScene(const std::string& filename);
	void loadBlueprint(const std::string& filename);

	bool initShaderSystem();
	void destroyShaderSystem();

private:
	std::string resourcesPath;
	DataLoader dataLoader;

	static std::map<std::string, SceneData*> sceneData;
	static std::map<std::string, GameObjectData*> blueprints;

	Ogre::FileSystemLayer* fileSystemLayer;
	Ogre::String mRTShaderLibPath;
	Ogre::RTShader::ShaderGenerator* mShaderGenerator; // The Shader generator instance.
	OgreBites::SGTechniqueResolverListener* mMaterialMgrListener; // Shader generator material manager listener.

};

#endif
