#pragma once
#ifndef CURSOR_H
#define CURSOR_H
#include "GaiaComponent.h"
#include "GaiaExports.h"

class Window;

class GAIA_API Cursor : public GaiaComponent
{
public:
	Cursor(GameObject* gameObject);
	virtual ~Cursor();

	virtual void handleData(ComponentData* data);

	/* Get position relative to window */
	void getPosition(int* x, int* y);

	/* Sets a position relative to window */
	void setPosition(int x, int y);

	/* Set system cursor visibility */
	void setVisibleOnWindow(bool visible);

private:
public:
private:
	Window* window;
	bool visibleOnWindow;
};

#endif