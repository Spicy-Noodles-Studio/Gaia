#pragma once

#ifndef GAIA_CORE_H
#define GAIA_CORE_H

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

#include "GameObject.h"
#include "Window.h"
#include "SoundSystem.h"

class GAIA_API GaiaCore
{
public:
	GaiaCore();
	~GaiaCore();

	void init();
	void run();
	void close();

private:
	Ogre::Root* root;
	Window* win;
	GameObject* obj;

	SoundSystem* sound = SoundSystem::GetInstance();
	
	void update();
};

#endif 
