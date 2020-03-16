#pragma once
#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "GaiaComponent.h"

namespace CEGUI {
	class Window;
}
typedef CEGUI::Window UIElement;

class GAIA_API UILayout : public GaiaComponent
{
private:
	UIElement* layout;

public:
	UILayout(GameObject* gameObject);
	~UILayout();

	virtual void handleData(ComponentData* data);

	void setLayout(const std::string& filename);
	UIElement* getElement(const std::string& name);
	void setVisible(bool visible);
};


#endif
