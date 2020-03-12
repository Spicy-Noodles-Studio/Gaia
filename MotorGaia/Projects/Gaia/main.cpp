#include <cstdlib>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#include <iostream>
#include "GaiaCore.h"

bool Clicked(const CEGUI::EventArgs& args)
{
	CEGUI::MouseEventArgs* args2 = (CEGUI::MouseEventArgs*)(&args);

	printf("clicked!\n");
	return false;
}

bool volumeEvent(const CEGUI::EventArgs& args)
{
	CEGUI::WindowEventArgs* args2 = (CEGUI::WindowEventArgs*)(&args);

	SoundSystem::GetInstance()->setSoundEffectsVolume(static_cast<CEGUI::Slider*>(args2->window)->getCurrentValue());

	return false;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(15450);

	// event test
	InterfaceSystem::registerEvent("clickevent", UIEvent(CEGUI::Window::EventMouseClick, &Clicked));
	InterfaceSystem::registerEvent("volumeEvent", UIEvent(CEGUI::Slider::EventValueChanged, &volumeEvent));

	GaiaCore engine;
	engine.init();

	engine.run();

	engine.close();


	return 0;
}