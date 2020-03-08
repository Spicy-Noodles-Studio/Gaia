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
#include "SoundEmitter.h"
#include "SoundListener.h"


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
	GameObject* obj,* aux;
	SoundEmitter* emi;
	SoundListener* lis;
	SoundSystem* sound = SoundSystem::GetInstance();
	
	void update();
};

#endif 
