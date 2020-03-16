
#ifdef _DEBUG
#include <cstdlib>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif

#include <iostream>
#include "GaiaCore.h"
#include "InterfaceSystem.h"
#include "DebugUtils.h"

bool Clicked(const CEGUI::EventArgs& args)
{
	CEGUI::MouseEventArgs* args2 = (CEGUI::MouseEventArgs*)(&args);

	LOG("clicked!\n");
	return false;
}

bool volumeEvent(const CEGUI::EventArgs& args)
{
	CEGUI::WindowEventArgs* args2 = (CEGUI::WindowEventArgs*)(&args);

	SoundSystem::GetInstance()->setSoundEffectsVolume(static_cast<CEGUI::Slider*>(args2->window)->getCurrentValue());

	return false;
}

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(94345);
#endif

	// event test
	InterfaceSystem::registerEvent("clickevent", UIEvent(CEGUI::Window::EventMouseClick, &Clicked));
	InterfaceSystem::registerEvent("volumeEvent", UIEvent(CEGUI::Slider::EventValueChanged, &volumeEvent));

	GaiaCore engine;
	engine.init();

	engine.run();

	engine.close();


	return 0;
}