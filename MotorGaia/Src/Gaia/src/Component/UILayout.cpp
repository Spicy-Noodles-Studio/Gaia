#include "UILayout.h"


#include "GameObject.h"
#include "ComponentData.h"

UILayout::UILayout(GameObject* gameObject) : GaiaComponent(gameObject), layout(nullptr)
{

}

UILayout::~UILayout()
{
	if (layout != nullptr)
		InterfaceSystem::GetInstance()->getRoot()->destroyChild(layout);
}

void UILayout::setLayout(const std::string& filename)
{
	layout = InterfaceSystem::GetInstance()->loadLayout(filename);
	if (layout == nullptr)
		return;
	InterfaceSystem::GetInstance()->getRoot()->addChild(layout);

	//esto esta MU FEO***

	layout->getChild("StaticImage")->setAlpha(0.0f);

	size_t index = 0;
	while (index < layout->getChild("StaticImage")->getChildCount())
	{
		layout->getChild("StaticImage")->getChildAtIdx(index)->setInheritsAlpha(false);
		++index;
	}
}

void UILayout::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		if (prop.first == "layout")
			setLayout(prop.second);
		else
			printf("UILAYOUT: invalid property name \"%s\"", prop.first.c_str());
	}
}

UIElement* UILayout::getElement(const std::string& name)
{
	if (layout == nullptr)
		return nullptr;
	return layout->getChild(name);
}

void UILayout::setVisible(bool visible)
{
	if (layout == nullptr)
		return;
	layout->setVisible(visible);
}