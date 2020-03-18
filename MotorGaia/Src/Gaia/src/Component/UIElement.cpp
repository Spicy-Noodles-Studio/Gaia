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
