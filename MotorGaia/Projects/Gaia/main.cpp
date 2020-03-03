#include <iostream>

#include "GaiaCore.h"

int main()
{
	GaiaCore engine;
	engine.init();
	
	engine.run();

	engine.close();

	return 0;
}