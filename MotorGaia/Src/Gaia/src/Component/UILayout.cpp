#include "UILayout.h"

#include <CEGUI/CEGUI.h>

#include "GameObject.h"
#include "ComponentData.h"
#include "InterfaceSystem.h"

UILayout::UILayout(GameObject* gameObject) : GaiaComponent(gameObject), layout(nullptr)
{

}

UILayout::~UILayout()
{
	if (layout != nullptr)
		InterfaceSystem::GetInstance()->getRoot()->getElement()->destroyChild(layout->getElement());
	delete layout;
}

void UILayout::setLayout(const std::string& filename)
{
	layout = InterfaceSystem::GetInstance()->loadLayout(filename);
	if (layout == nullptr)
		return;
	InterfaceSystem::GetInstance()->getRoot()->getElement()->addChild(layout->getElement());

	layout->getElement()->getChild("StaticImage")->setAlpha(0.0f);

	size_t index = 0;
	while (index < layout->getElement()->getChild("StaticImage")->getChildCount())
	{
		layout->getElement()->getChild("StaticImage")->getChildAtIdx(index)->setInheritsAlpha(false);
		++index;
	}
}

void UILayout::setEvent(const std::string& element, const std::string& event)
{
	getUIElement("StaticImage").getElement()->getChild(element)->
		subscribeEvent(InterfaceSystem::GetInstance()->getEvent(event).first, InterfaceSystem::GetInstance()->getEvent(event).second);
}

void UILayout::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "layout")
			setLayout(prop.second);
		else if (prop.first == "event")
		{
			std::string element;
			std::string event;
			char c;
			while (ss >> element >> event)
			{
				setEvent(element, event);
				if (ss)
					ss >> c;
			}
		}
		else
			LOG("UILAYOUT: invalid property name \"%s\"", prop.first.c_str());
	}
}

UIElement UILayout::getUIElement(const std::string& name)
{
	if (layout == nullptr)
		return nullptr;
	return UIElement(layout->getElement()->getChild(name));
}

void UILayout::setVisible(bool visible)
{
	if (layout == nullptr)
		return;
	layout->getElement()->setVisible(visible);
}