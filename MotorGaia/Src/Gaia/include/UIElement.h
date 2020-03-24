#pragma once
#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "GaiaExports.h"
#include "CEGUI/CEGUI.h"

class GAIA_API UIElement
{
private:
	CEGUI::Window* element;

public:
	UIElement(CEGUI::Window* element);
	~UIElement();

	CEGUI::Window* getElement();

	void setText(const std::string& text);

	void setPosition(float x, float y);
	void setPosition(int x, int y);

	void setSize(float x, float y);
	void setSize(int x, int y);

	UIElement getChild(const std::string& name);
	UIElement createChild(const std::string& type, const std::string& name);

	void setVisible(bool visible);

	void flipHorizontal();
};


#endif
