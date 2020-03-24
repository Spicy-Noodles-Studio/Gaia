#include "UIElement.h"

UIElement::UIElement(CEGUI::Window* element) : element(element)
{

}

UIElement::~UIElement()
{

}

CEGUI::Window* UIElement::getElement()
{
	return element;
}

void UIElement::setText(const std::string& text)
{
	element->setText(text);
}

void UIElement::setPosition(float x, float y)
{
	element->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
}

void UIElement::setPosition(int x, int y)
{
	element->setPosition(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
}

void UIElement::setSize(float x, float y)
{
	element->setSize(CEGUI::USize(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
}

void UIElement::setSize(int x, int y)
{
	element->setSize(CEGUI::USize(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
}

UIElement UIElement::getChild(const std::string& name)
{
	return element->getChild(name);
}

UIElement UIElement::createChild(const std::string& type, const std::string& name)
{
	element->addChild(CEGUI::WindowManager::getSingleton().createWindow(type, name));
	return element->getChild(name);
}

void UIElement::setVisible(bool visible)
{
	element->setVisible(visible);
}

void UIElement::flipHorizontal()
{
	element->getGeometryBuffer().setPivot(CEGUI::Vector3f(element->getPixelSize().d_width / 2.0f, 0.0f, 0.0f));
	element->getGeometryBuffer().setRotation( CEGUI::Quaternion::eulerAnglesDegrees(0.0f, 180.0f, 0.0f));
}
