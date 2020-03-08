#include "ResourcesManager.h"
#include <fstream>

#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreTextureManager.h>
#include <OgreGpuProgramManager.h>


std::map<std::string, SceneData*> ResourcesManager::sceneData;
std::map<std::string, GameObjectData*> ResourcesManager::blueprints;

ResourcesManager::ResourcesManager(const std::string& filePath) : dataLoader(), resourcesPath(filePath), fileSystemLayer(nullptr), 
																	shaderLibPath(""), shaderGenerator(nullptr), shaderTechniqueResolver(nullptr)
{

}

ResourcesManager::~ResourcesManager()
{
	// Close method should be called externally
	close();
}


void ResourcesManager::init()
{
	// Init Ogre SubSystem
	fileSystemLayer = new Ogre::FileSystemLayer("./");
	initShaderSystem();

	// Pair: (Types, Filepath)
	std::vector<std::pair<std::string, std::string>> filePaths;
	// Reads all file paths
	std::fstream file(resourcesPath);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: Resources filepath not valid: %s\n", resourcesPath.c_str());
		return;
	}
	printf("Reading Resources paths...\n");
	// Start reading resources
	std::string type, c, filename;
	while (file >> type >> c >> filename) {
		if (c != "=") {
			printf("RESOURCES MANAGER: invalid resources file format\nFailed at: %s %s %s\n", type.c_str(), c.c_str(), filename.c_str());
			file.close();
			return;
		}
		filePaths.push_back({ type, filename });
		// Reset variables
		type = c = filename = "";
	}
	file.close();

	// Start loading resources
	for (int i = 0; i < filePaths.size(); i++) {
		std::string type = filePaths[i].first;
		std::string path = filePaths[i].second;
		loadResources(type, path);
	}

	//Initialize (REMEMBER TO INITIALIZE OGRE)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourcesManager::close()
{
	for (auto s : sceneData)
		delete s.second;
	for (auto b : blueprints)
		delete b.second;

	sceneData.clear();
	blueprints.clear();

	if(fileSystemLayer != nullptr)
		delete fileSystemLayer;
	fileSystemLayer = nullptr;

	destroyShaderSystem();
}

void ResourcesManager::loadScene(const std::string& filename)
{
	bool loaded = true;
	SceneData* data = dataLoader.loadScene(filename, loaded);
	if (!registerSceneData(data)) { delete data; loaded = false; }
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Scene, filename %s.\n", filename.c_str());
}

void ResourcesManager::loadBlueprint(const std::string& filename)
{
	bool loaded = true;
	GameObjectData* data = dataLoader.loadBlueprint(filename, loaded);
	if (!registerBlueprint(data)) { delete data; loaded = false; }
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Blueprint, filename %s.\n", filename.c_str());
}

bool ResourcesManager::initShaderSystem()
{
	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		// Core shader libs not found -> shader generating will fail.
		if (shaderLibPath.empty()) {
			printf("RESOURCES MANAGER: Shader libs not found.\n");
			return false;
		}
		// Create and register the material manager listener if it doesn't exist yet.
		if (!shaderTechniqueResolver) {
			shaderTechniqueResolver = new ShaderTechniqueResolver(shaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(shaderTechniqueResolver);
		}
	}
	return true;
}

void ResourcesManager::destroyShaderSystem()
{
	// Restore default scheme.
	if(Ogre::MaterialManager::getSingletonPtr() != nullptr)
		Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

	// Unregister the material manager listener.
	if (shaderTechniqueResolver != nullptr)
	{
		Ogre::MaterialManager::getSingleton().removeListener(shaderTechniqueResolver);
		delete shaderTechniqueResolver;
		shaderTechniqueResolver = nullptr;
	}

	// Destroy RTShader system.
	if (shaderGenerator != nullptr)
	{
		Ogre::RTShader::ShaderGenerator::destroy();
		shaderGenerator = nullptr;
	}
}

bool ResourcesManager::registerSceneData(SceneData* data)
{
	if (sceneData.find(data->name) != sceneData.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing SceneData: %s.\n", data->name.c_str());
		return false;
	}
	sceneData[data->name] = data;
	return true;
}

bool ResourcesManager::registerBlueprint(GameObjectData* data)
{
	if (blueprints.find(data->name) != blueprints.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing Blueprint: %s.\n", data->name.c_str());
		return false;
	}
	blueprints[data->name] = data;
	return true;
}

const SceneData* ResourcesManager::getSceneData(const std::string& name)
{
	if (sceneData.find(name) == sceneData.end()) {
		printf("RESOURCES MANAGER: trying to get not existing SceneData: %s.\n", name.c_str());
		return nullptr;
	}
	return sceneData[name];
}

const SceneData* ResourcesManager::getSceneData(int index)
{
	if (index >= sceneData.size()) {
		printf("RESOURCES MANAGER: trying to get not existing SceneData index: %i.\n", index);
		return nullptr;
	}
	auto it = sceneData.begin(); 
	std::advance(it, index);
	return (*it).second;
}

const GameObjectData* ResourcesManager::getBlueprint(const std::string& name)
{
	if (blueprints.find(name) == blueprints.end()) {
		printf("RESOURCES MANAGER: trying to get not existing Blueprint: %s.\n", name.c_str());
		return nullptr;
	}
	return blueprints[name];
}

void ResourcesManager::loadResources(const std::string& resourceType, const std::string& path)
{
	if (resourceType == "Scenes")
		loadScenes(path);
	else if (resourceType == "Blueprints")
		loadBlueprints(path);
	else if (resourceType == "Ogre")
		loadOgreResources(path);
	else
		printf("RESOURCES MANAGER: invalid resource type: %s. Resource not loaded.\n", resourceType.c_str());
}

void ResourcesManager::loadScenes(const std::string& filename)
{
	std::fstream file(filename);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: ScenesAssets path %s not found.\n", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	for (std::string pName : paths)
		loadScene(pName);
}

void ResourcesManager::loadBlueprints(const std::string& filename)
{
	std::fstream file(filename);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: BlueprintsAssets path %s not found.\n", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	for (std::string pName : paths)
		loadBlueprint(pName);
}

void ResourcesManager::loadOgreResources(const std::string& filename)
{
	// load resource paths from config file
	Ogre::ConfigFile cf;

	Ogre::String resourcesPath = fileSystemLayer->getConfigFilePath(filename);
	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{
		cf.load(resourcesPath);
	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(/*mSolutionPath +*/ "\\Assets"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}

	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();

	// Add locations for supported shader languages
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/GLSL120", type, sec);

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/GLSL150", type, sec);
		}
		else
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/GLSL", type, sec);
		}

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/GLSL400", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/Shaders/programs/HLSL", type, sec);
	}

	std::string shaderPath = arch + "/Shaders/programs/RTShaderLib";
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(shaderPath + "/materials", type, sec);

	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(shaderPath + "/GLSL", type, sec);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(shaderPath + "/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(shaderPath + "/GLSL", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(shaderPath + "/HLSL", type, sec);
	}
}
