#include <iostream>

#include "GaiaCore.h"
#include "ComponentManager.h"
#include "GaiaComponent.h"
#include "UserComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ResourcesManager.h"


int main() 
{
	GaiaCore engine;
	engine.init();
	
	engine.run();

	engine.close();

	return 0;
}