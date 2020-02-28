#include "GaiaCore.h"

int main()
{

	GaiaCore engine;

	engine.setupResources();
	engine.init();
	engine.run();

	return 0;
}