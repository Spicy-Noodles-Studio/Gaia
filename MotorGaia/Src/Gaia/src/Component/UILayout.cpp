#include "UILayout.h"

#include <CEGUI/CEGUI.h>
#include "InterfaceSystem.h"
#include "GameObject.h"
#include "ComponentData.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(UILayout);

UILayout::UILayout(GameObject* gameObject) : GaiaComponent(gameObject), interfaceSystem(nullptr), layout(nullptr)
{
	interfaceSystem = InterfaceSystem::GetInstance();
}

UILayout::~UILayout()
{
	if (layout != nullptr) {
		checkNullAndBreak(interfaceSystem);
		UIElement* root = interfaceSystem->getRoot();
		checkNullAndBreak(root);
		CEGUI::Window* element = root->getElement();
		checkNullAndBreak(element);
		checkNullAndBreak(layout->getElement());

		element->destroyChild(layout->getElement());
		delete layout;
	}
}

void UILayout::setLayout(const std::string& filename)
{
	checkNullAndBreak(interfaceSystem);
	layout = interfaceSystem->loadLayout(filename);
	checkNullAndBreak(layout);

	UIElement* root = interfaceSystem->getRoot();
	checkNullAndBreak(root);

	CEGUI::Window* element = root->getElement();
	checkNullAndBreak(element);

	CEGUI::Window* layoutElement = layout->getElement();
	checkNullAndBreak(layoutElement);

	element->addChild(layoutElement);
	layoutElement->setAlpha(0.0f);

	size_t index = 0;
	while (index < layoutElement->getChildCount())
	{
		CEGUI::Window* child = layoutElement->getChildAtIdx(index);
		if(child != nullptr)
			child->setInheritsAlpha(false);
		++index;
	}
}

UIElement UILayout::getUIElement(const std::string& name)
{
	checkNullAndBreak(layout, nullptr);

	CEGUI::Window* element = layout->getElement();
	checkNullAndBreak(element, nullptr);

	CEGUI::Window* child = nullptr;
	try {
		child = element->getChild(name);
	}
	catch (std::exception exception){
		LOG_ERROR("UILAYOUT", "Error trying to get a child");
		return nullptr;
	}

	return UIElement(child);
}

UIElement UILayout::getRoot()
{
	checkNullAndBreak(layout, nullptr);
	checkNullAndBreak(layout->getElement(), nullptr);

	return *layout;
}

void UILayout::setVisible(bool visible)
{
	checkNullAndBreak(layout);

	CEGUI::Window* element = layout->getElement();
	checkNullAndBreak(element);

	element->setVisible(visible);
}

void UILayout::setEvent(const std::string& element, const std::string& event)
{
	checkNullAndBreak(interfaceSystem);

	checkNullAndBreak(layout);
	CEGUI::Window* layoutElement = layout->getElement();
	checkNullAndBreak(layoutElement);

	CEGUI::Window* child = layoutElement->getChildRecursive(element);
	checkNullAndBreak(child);

	UIEvent uiEvent = interfaceSystem->getEvent(event);
	CEGUI::String eventType = interfaceSystem->getEventType(uiEvent.first);

	CEGUI::Event::Connection connection = child->subscribeEvent(eventType, uiEvent.second);
	if (!connection.isValid())
		LOG_ERROR("UILAYOUT", "Event set is not valid");
}

void UILayout::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

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
			LOG_ERROR("UILAYOUT", "Invalid property name \"%s\"", prop.first.c_str());
	}
}
