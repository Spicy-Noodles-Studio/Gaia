#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#pragma once

#include "Singleton.h"

class RenderSystem : public Singleton<RenderSystem> 
{
private:
	int a;

public:
	void printA();
	RenderSystem();

};

#endif