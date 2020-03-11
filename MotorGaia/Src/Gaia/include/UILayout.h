#pragma once
#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "GaiaComponent.h"
#include <CEGUI/CEGUI.h>
#include "InterfaceSystem.h"

class UILayout : public GaiaComponent
{
private:
	CEGUI::Window* layout;

public:
	UILayout(GameObject* gameObject);
	~UILayout();

	virtual void handleData(ComponentData* data);

	void setLayout(const std::string& filename);
	UIElement* getElement(const std::string& name);
	void setVisible(bool visible);
};


#endif
