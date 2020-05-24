#include "ResourcesManager.h"
#include <fstream>
#include "DebugUtils.h"

#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreTextureManager.h>
#include <OgreGpuProgramManager.h>

std::map<std::string, std::pair<int, int>> ResourcesManager::sceneDataIndexes;
std::vector<SceneData*> ResourcesManager::sceneData;
std::map<std::string, BlueprintData*> ResourcesManager::blueprintData;
std::map<std::string, Sound*> ResourcesManager::sounds;

ResourcesManager::ResourcesManager(const std::string& filePath) :	resourcesPath(filePath), fileSystemLayer(nullptr), 
																	shaderLibPath(""), shaderGenerator(nullptr), shaderTechniqueResolver(nullptr)
{

}

ResourcesManager::~ResourcesManager()
{
	// Close method should be called externally
	//close();
}


void ResourcesManager::init()
{
	// Init Ogre SubSystem
	fileSystemLayer = new Ogre::FileSystemLayer("./");
	initShaderSystem();

	// Pair: (Types, Filepath)
	std::vector<std::pair<std::string, std::string>> filePaths;
	// Reads all file paths
	std::ifstream file(resourcesPath);
	if (!file.is_open()) {
		LOG_ERROR("RESOURCES MANAGER", "Resources filepath not valid: %s", resourcesPath.c_str());
		return;
	}
	LOG("Reading Resources paths...");
	// Start reading resources
	std::string type, c, filename;
	while (file >> type >> c >> filename) {
		if (c != "=") {
			LOG_ERROR("RESOURCES MANAGER", "Invalid resources file format\nFailed at: %s %s %s", type.c_str(), c.c_str(), filename.c_str());
			file.close();
			return;
		}
		filePaths.push_back({ type, filename });
		// Reset variables
		type = c = filename = "";
	}
	file.close();

	// Start loading resources
	/* Code for main thread */
	/*for (int i = 0; i < filePaths.size(); i++) {
		std::string type = filePaths[i].first;
		std::string path = filePaths[i].second;
		loadResources(type, path);
	}*/

	/* Code for multithreading */
	std::vector<std::thread> resourceThreads;
	for (int i = 0; i < filePaths.size(); i++) {
		//if(filePaths[i].first != "Ogre") // Little hack, OgreResources does not load correctly on multithreading
			resourceThreads.push_back(std::thread(&ResourcesManager::locateResourceType, std::ref(*this), filePaths[i].first, filePaths[i].second));
		//else
		//	locateResourceType(filePaths[i].first, filePaths[i].second);
	}

	for (int i = 0; i < resourceThreads.size(); i++) {
		resourceThreads[i].join();
	}

	//When located, initialize
	initializeAllResources();
}

void ResourcesManager::close()
{
	for (auto sData : sceneData)
		delete sData;
		
	for (auto bData : blueprintData)
		delete bData.second;

	for (auto sound : sounds) {
		sound.second->release();
		sound.second = nullptr;
	}

	sceneData.clear();
	blueprintData.clear();
	sounds.clear();

	if(fileSystemLayer != nullptr)
		delete fileSystemLayer;
	fileSystemLayer = nullptr;

	destroyShaderSystem();
}


void ResourcesManager::locateScene(const std::string& filename, int index)
{
	SceneData* sceneData = new SceneData();
	sceneData->locate(filename);
	sceneData->index = index;

	if (sceneData->getLoadState() == Loadable::LoadState::INVALID) {
		delete sceneData;
		return;
	}

	if (!registerSceneData(sceneData)) { 
		delete sceneData; 
		LOG_ERROR("RESOURCES MANAGER","Error registring Scene %s", filename.c_str());
	}
}

void ResourcesManager::locateBlueprint(const std::string& filename)
{
	BlueprintData* bpData = new BlueprintData();
	bpData->locate(filename);

	if (bpData->getLoadState() == Loadable::LoadState::INVALID) {
		delete bpData;
		return;
	}

	if (!registerBlueprint(bpData)) {
		delete bpData;
		LOG_ERROR("RESOURCES MANAGER", "Error registring Blueprint %s", filename.c_str());
	}
}

void ResourcesManager::loadSound(const std::string& filename)
{
	SoundSystem* soundSystem = SoundSystem::GetInstance();
	if (soundSystem == nullptr) {
		LOG_ERROR("RESOURCES MANAGER", "SoundSystem reference is nullptr, %s not loaded", filename.c_str());
		return;
	}
	// Lectura de linea
	std::string soundfile, name, soundmode, loop;
	std::stringstream ss(filename);
	if (!(ss >> soundfile >> name >> soundmode >> loop)) {
		LOG_ERROR("RESOURCES MANAGER", "Invalid format \"%s\"", filename.c_str());
		return;
	}
	SoundMode mode = FMOD_DEFAULT;
	mode = soundmode == "3D" ? FMOD_3D : FMOD_2D;
	mode |= loop == "true" ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= FMOD_NONBLOCKING; //Load in the background
	//mode |= FMOD_CREATESTREAM;

	FMOD::Sound* sound;
	if ((sound = soundSystem->createSound(soundfile, mode)) == nullptr) {
		LOG_ERROR("RESOURCES MANAGER", "Sound named \"%s\" not found", soundfile.c_str());
		return;
	}

	// lock_guard secures unlock on destruction
	std::lock_guard<std::mutex> lock(soundMutex);
	sounds[name] = sound;

	LOG("Sound loaded: %s", soundfile.c_str());
}

void ResourcesManager::initializeAllResources()
{
	std::vector<std::thread> threads;

	//Ogre
	initializeOgreResources();

	//Scenes
	threads.push_back(std::thread(&ResourcesManager::initializeScenes, std::ref(*this)));

	//Blueprints
	threads.push_back(std::thread(&ResourcesManager::initializeBlueprints, std::ref(*this)));

	//Sounds
	//threads.push_back(std::thread(&ResourcesManager::initializeSounds, std::ref(*this)));

	//Wait
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
}

void ResourcesManager::initializeOgreResources()
{
	//Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourcesManager::initializeScenes()
{
	for (auto data : sceneData) {
		if (data == nullptr) continue;
		data->loadAsync();
	}
}

void ResourcesManager::initializeBlueprints()
{
	for (auto data : blueprintData) {
		if (data.second == nullptr) continue;
		data.second->loadAsync();
	}
}

void ResourcesManager::initializeSounds()
{
	// FMOD does it for us
}

bool ResourcesManager::initShaderSystem()
{
	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		// Core shader libs not found -> shader generating will fail.
		if (shaderLibPath.empty()) {
			LOG_ERROR("RESOURCES MANAGER", "Shader libs not found");
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
	if (data == nullptr || data->getLoadState() == Loadable::LoadState::INVALID) {
		LOG_ERROR("RESOURCES MANAGER", "SceneData reference is null or not valid");
		return false;
	}
	// lock_guard secures unlock on destruction
	std::lock_guard<std::mutex> lock(sceneDataMutex);
	if (sceneDataIndexes.find(data->id) != sceneDataIndexes.end()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to add an already existing SceneData: %s", data->id.c_str());
		return false;
	}
	sceneData.push_back(data);
	sceneDataIndexes[data->id] = { data->index, sceneData.size() - 1 };

	return true;
}

bool ResourcesManager::registerBlueprint(BlueprintData* data)
{
	if (data == nullptr || data->getLoadState() == Loadable::LoadState::INVALID) {
		LOG_ERROR("RESOURCES MANAGER", "BlueprintData reference is null or not valid");
		return false;
	}
	// lock_guard secures unlock on destruction
	std::lock_guard<std::mutex> lock(blueprintMutex);
	if (blueprintData.find(data->id) != blueprintData.end()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to add an already existing Blueprint: %s", data->id.c_str());
		return false;
	}
	blueprintData[data->id] = data;
	return true;
}

const SceneData* ResourcesManager::getSceneData(const std::string& name)
{
	if (sceneDataIndexes.find(name) == sceneDataIndexes.end()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to get not existing SceneData: %s", name.c_str());
		return nullptr;
	}
	return sceneData[sceneDataIndexes[name].second];
}

const SceneData* ResourcesManager::getSceneData(int index)
{
	if (index >= sceneDataIndexes.size()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to get not existing SceneData index: %i", index);
		return nullptr;
	}

	for (auto scene : sceneDataIndexes) {
		if (scene.second.first == index)
			return sceneData[scene.second.second];
	}

	return nullptr;
}

const BlueprintData* ResourcesManager::getBlueprint(const std::string& name)
{
	if (blueprintData.find(name) == blueprintData.end()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to get not existing Blueprint: %s", name.c_str());
		return nullptr;
	}
	return blueprintData[name];
}

Sound* ResourcesManager::getSound(const std::string& name)
{
	if (sounds.find(name) == sounds.end()) {
		LOG_ERROR("RESOURCES MANAGER", "Trying to get not existing Sound: %s", name.c_str());
		return nullptr;
	}
	return sounds[name];
}

void ResourcesManager::locateResourceType(const std::string& resourceType, const std::string& path)
{
	if (resourceType == "Scenes")
		locateScenes(path);
	else if (resourceType == "Blueprints")
		locateBlueprints(path);
	else if (resourceType == "Sounds")
		locateSounds(path);
	else if (resourceType == "Ogre")
		locateOgreResources(path);
	else
		LOG_ERROR("RESOURCES MANAGER", "Invalid resource type: \"%s\". Resource not loaded", resourceType.c_str());
}

void ResourcesManager::locateScenes(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		LOG_ERROR("RESOURCES MANAGER", "ScenesAssets path %s not found", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	/*Code to load in the main thread*/
	/*for (std::string pName : paths)
		loadScene(pName);*/

	/* Code to load using multithreading */
	std::vector<std::thread> threads;
	for (int i = 0; i < paths.size(); i++) 
		threads.push_back(std::thread(&ResourcesManager::locateScene, std::ref(*this), paths[i], i));

	//Wait to finish
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();

}

void ResourcesManager::locateBlueprints(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		LOG_ERROR("RESOURCES MANAGER", "BlueprintsAssets path %s not found", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	/*for (std::string pName : paths)
		loadBlueprint(pName);*/

	/* Code to load using multithreading */
	std::vector<std::thread> threads;
	for (int i = 0; i < paths.size(); i++)
		threads.push_back(std::thread(&ResourcesManager::locateBlueprint, std::ref(*this), paths[i]));

	//Wait to finish
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
}

void ResourcesManager::locateSounds(const std::string& filename)
{
	std::ifstream stream;
	stream.open(filename);
	if (!stream.is_open()) {
		LOG_ERROR("RESOURCES MANAGER", "SoundsAssets path %s not found", filename.c_str());
		return;
	}
	std::string line;
	// Skip first two lines (Usage & example)
	std::getline(stream, line);
	std::getline(stream, line);

	std::vector<std::string> lines;
	while (std::getline(stream, line))
		lines.push_back(line);		
	stream.close();

	/* Multi-threading */
	std::vector<std::thread> threads;
	for (int i = 0; i < lines.size(); i++)
		threads.push_back(std::thread(&ResourcesManager::loadSound, std::ref(*this), lines[i]));

	for (int i = 0; i < threads.size(); i++)
		threads[i].join();

}

void ResourcesManager::locateOgreResources(const std::string& filename)
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

	//Initialize (REMEMBER TO INITIALIZE OGRE)
	LOG("RESOURCES MANAGER: Ogre resources loaded");
}
