#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

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