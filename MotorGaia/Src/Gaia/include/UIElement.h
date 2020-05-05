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

	void setProperty(const std::string& prop, const std::string& value);

	void setPosition(float x, float y);
	void setPosition(int x, int y);

	void setSize(float x, float y);
	void setSize(int x, int y);

	UIElement getChild(const std::string& name);
	UIElement getChildAtIndex(int index);
	UIElement createChild(const std::string& type, const std::string& name);
	int getChildCount();

	void setVisible(bool visible);
	bool isVisible();

	void flipHorizontal();

	void setInheritsAlpha(bool inherits);
	void setAlpha(float alpha);

	float getScrollPositionScrollBar();
	float setScrollPositionScrollBar(float value);

	float setCheckBoxState(bool value);

};


#endif
