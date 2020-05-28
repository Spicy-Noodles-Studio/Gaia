#pragma once
#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "GaiaComponent.h"
#include "UIElement.h"

class InterfaceSystem;

class GAIA_API UILayout : public GaiaComponent
{
private:
	InterfaceSystem* interfaceSystem;
	UIElement* layout;

public:
	UILayout(GameObject* gameObject);
	virtual ~UILayout();

	void setLayout(const std::string& filename);
	UIElement getUIElement(const std::string& name);
	UIElement getRoot();
	void setVisible(bool visible);

private:
	void setEvent(const std::string& element, const std::string& event);

protected:
	virtual void handleData(ComponentData* data);
};


#endif
