#include <cstdlib>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#include <iostream>
#include "GaiaCore.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(15450);

	GaiaCore engine;
	engine.init();

	engine.run();

	engine.close();


	return 0;
}