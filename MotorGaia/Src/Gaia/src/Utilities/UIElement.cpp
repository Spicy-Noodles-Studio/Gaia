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

void UIElement::setProperty(const std::string& prop, const std::string& value)
{
	element->setProperty(prop, value);
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

UIElement UIElement::getChildAtIndex(int index)
{
	return element->getChildAtIdx(index);
	
}

UIElement UIElement::createChild(const std::string& type, const std::string& name)
{
	element->addChild(CEGUI::WindowManager::getSingleton().createWindow(type, name));
	return element->getChild(name);
}

int UIElement::getChildCount()
{
	return element->getChildCount();
}

void UIElement::setVisible(bool visible)
{
	element->setVisible(visible);
}

bool UIElement::isVisible()
{
	return element->isVisible();
}

void UIElement::setAlwaysOnTop(bool top)
{
	element->setAlwaysOnTop(true);
}

void UIElement::setEnabled(bool enable)
{
	if (!enable)
		element->disable();
	else
		element->enable();
}

void UIElement::flipHorizontal()
{
	element->getGeometryBuffer().setPivot(CEGUI::Vector3f(element->getPixelSize().d_width / 2.0f, 0.0f, 0.0f));
	element->getGeometryBuffer().setRotation( CEGUI::Quaternion::eulerAnglesDegrees(0.0f, 180.0f, 0.0f));
}

void UIElement::setInheritsAlpha(bool inherits)
{
	element->setInheritsAlpha(inherits);
}

void UIElement::setAlpha(float alpha)
{
	element->setAlpha(alpha);
}

float UIElement::getScrollPositionScrollBar()
{
	if (element->getWindowRendererName() == "Core/Scrollbar") {
		CEGUI::Scrollbar* x = static_cast<CEGUI::Scrollbar*>(element);
		return x->getScrollPosition();
	}
	else
		return -1;
	
	
}

float UIElement::setScrollPositionScrollBar(float value)
{
	if (element->getWindowRendererName() == "Core/Scrollbar") {
		CEGUI::Scrollbar* x = static_cast<CEGUI::Scrollbar*>(element);
		x->setScrollPosition(value);
		return 0;
	}
	else
		return -1;
}

float UIElement::setCheckBoxState(bool value)
{
	if (element->getWindowRendererName() == "Core/ToggleButton") {
		CEGUI::ToggleButton* x = static_cast<CEGUI::ToggleButton*>(element);
	//	x->setPushedState(value);
		x->setSelected(value);
		return 0;
	}
	else
		return -1;
}
