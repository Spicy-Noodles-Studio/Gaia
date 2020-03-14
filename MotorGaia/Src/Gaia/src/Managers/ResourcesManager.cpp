#include "ResourcesManager.h"
#include <fstream>

#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreTextureManager.h>
#include <OgreGpuProgramManager.h>

std::map<std::string, SceneData*> ResourcesManager::sceneData;
std::map<std::string, GameObjectData*> ResourcesManager::blueprintData;
std::map<std::string, Sound*> ResourcesManager::sounds;

ResourcesManager::ResourcesManager(const std::string& filePath) :	dataLoader(), resourcesPath(filePath), fileSystemLayer(nullptr), 
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
	/* Code for main thread */
	/*for (int i = 0; i < filePaths.size(); i++) {
		std::string type = filePaths[i].first;
		std::string path = filePaths[i].second;
		loadResources(type, path);
	}*/

	/* Code for multithreading */
	std::vector<std::thread> resourceThreads;
	for (int i = 0; i < filePaths.size(); i++) {
		resourceThreads.push_back(std::thread(&ResourcesManager::loadResources, std::ref(*this), filePaths[i].first, filePaths[i].second));
	}

	for (int i = 0; i < resourceThreads.size(); i++) {
		resourceThreads[i].join();
	}

	//Initialize (REMEMBER TO INITIALIZE OGRE)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourcesManager::close()
{
	for (auto sData : sceneData)
		delete sData.second;
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

void ResourcesManager::loadScene(const std::string& filename)
{
	bool loaded = true;
	SceneData* data = dataLoader.loadScene(filename, loaded);
	if (!registerSceneData(data)) { delete data; loaded = false; }
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Scene, filename %s\n", filename.c_str());
	else
		printf("RESOURCES MANAGER: Scene file \"%s\" loaded\n", filename.c_str());
}

void ResourcesManager::loadBlueprint(const std::string& filename)
{
	bool loaded = true;
	GameObjectData* data = dataLoader.loadBlueprint(filename, loaded);
	if (!registerBlueprint(data)) { delete data; loaded = false; }
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Blueprint, filename %s\n", filename.c_str());
	else
		printf("RESOURCES MANAGER: Blueprint file \"%s\" loaded\n", filename.c_str());
}

void ResourcesManager::loadSound(const std::string& filename)
{
	// Lectura de linea
	std::string soundfile, name, soundmode, loop;
	std::istringstream iss(filename);
	if (!(iss >> soundfile >> name >> soundmode >> loop)) {
		printf("RESOURCES MANAGER: invalid format \"%s\"\n", filename.c_str());
		return;
	}
	SoundMode mode = FMOD_DEFAULT;
	mode = soundmode == "3D" ? FMOD_3D : FMOD_2D;
	mode |= loop == "true" ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	//mode |= FMOD_CREATESTREAM;

	FMOD::Sound* sound;
	if ((sound = SoundSystem::GetInstance()->createSound(soundfile, mode)) == nullptr) {
		printf("RESOURCES MANAGER: Sound named \"%s\" not found\n", soundfile.c_str());
		return;
	}

	soundMutex.lock();
	sounds[name] = sound;
	soundMutex.unlock();

	printf("RESOURCES MANAGER: Sound loaded: %s\n", soundfile.c_str());
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
	// lock_guard secures unlock on destruction
	std::lock_guard<std::mutex> lock(sceneDataMutex);
	if (sceneData.find(data->name) != sceneData.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing SceneData: %s.\n", data->name.c_str());
		return false;
	}
	sceneData[data->name] = data;

	return true;
}

bool ResourcesManager::registerBlueprint(GameObjectData* data)
{
	// lock_guard secures unlock on destruction
	std::lock_guard<std::mutex> lock(blueprintMutex);
	if (blueprintData.find(data->name) != blueprintData.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing Blueprint: %s.\n", data->name.c_str());
		return false;
	}
	blueprintData[data->name] = data;
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
	if (blueprintData.find(name) == blueprintData.end()) {
		printf("RESOURCES MANAGER: trying to get not existing Blueprint: %s.\n", name.c_str());
		return nullptr;
	}
	return blueprintData[name];
}

Sound* ResourcesManager::getSound(const std::string& name)
{
	if (sounds.find(name) == sounds.end()) {
		printf("RESOURCES MANAGER: trying to get not existing Sound: %s.\n", name.c_str());
		return nullptr;
	}
	return sounds[name];
}

void ResourcesManager::loadResources(const std::string& resourceType, const std::string& path)
{
	if (resourceType == "Scenes")
		loadScenes(path);
	else if (resourceType == "Blueprints")
		loadBlueprints(path);
	else if (resourceType == "Sounds")
		loadSounds(path);
	else if (resourceType == "Ogre")
		loadOgreResources(path);
	else
		printf("RESOURCES MANAGER: invalid resource type: \"%s\". Resource not loaded.\n", resourceType.c_str());
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

	/*Code to load in the main thread*/
	/*for (std::string pName : paths)
		loadScene(pName);*/

	/* Code to load using multithreading */
	std::vector<std::thread> threads;
	for (int i = 0; i < paths.size(); i++) 
		threads.push_back(std::thread(&ResourcesManager::loadScene, std::ref(*this), paths[i]));

	//Wait to finish
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();

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

	/*for (std::string pName : paths)
		loadBlueprint(pName);*/

	/* Code to load using multithreading */
	std::vector<std::thread> threads;
	for (int i = 0; i < paths.size(); i++)
		threads.push_back(std::thread(&ResourcesManager::loadBlueprint, std::ref(*this), paths[i]));

	//Wait to finish
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
}

void ResourcesManager::loadSounds(const std::string& filename)
{
	std::fstream stream;
	stream.open(filename);
	if (!stream.is_open()) {
		printf("RESOURCES MANAGER: SoundsAssets path %s not found.\n", filename.c_str());
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

	printf("RESOURCES MANAGER: Ogre resources loaded\n");
}

void ResourcesManager::loadInterfaceResources(const std::string& filename)
{
}
