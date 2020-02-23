#pragma once

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

class GAIA_API GaiaCore
{
public:
	GaiaCore();

	void init();
	void update();
};

