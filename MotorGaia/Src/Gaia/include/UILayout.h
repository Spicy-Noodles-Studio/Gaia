#pragma once
#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "GaiaComponent.h"
#include "UIElement.h"

class GAIA_API UILayout : public GaiaComponent
{
private:
	UIElement* layout;

	void setEvent(const std::string& element, const std::string& event);

public:
	UILayout(GameObject* gameObject);
	~UILayout();

	virtual void handleData(ComponentData* data);

	void setLayout(const std::string& filename);
	UIElement getUIElement(const std::string& name);
	void setVisible(bool visible);
};


#endif
