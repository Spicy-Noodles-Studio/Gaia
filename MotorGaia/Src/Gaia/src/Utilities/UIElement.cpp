#include "UIElement.h"

UIElement::UIElement() : element(nullptr)
{
}

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
	checkNullAndBreak(element);
	element->setText(text);
}

void UIElement::setProperty(const std::string& prop, const std::string& value)
{
	checkNullAndBreak(element);
	element->setProperty(prop, value);
}

void UIElement::setPosition(float x, float y)
{
	checkNullAndBreak(element);
	element->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
}

void UIElement::setPosition(int x, int y)
{
	checkNullAndBreak(element);
	element->setPosition(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
}

void UIElement::setSize(float x, float y)
{
	checkNullAndBreak(element);
	element->setSize(CEGUI::USize(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
}

void UIElement::setSize(int x, int y)
{
	checkNullAndBreak(element);
	element->setSize(CEGUI::USize(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
}

UIElement UIElement::getChild(const std::string& name)
{
	checkNullAndBreak(element, UIElement());
	try {
		UIElement child = UIElement(element->getChild(name));
		return child;
	}
	catch (std::exception exception) {
		LOG_ERROR("UI ELEMENT", "Error trying to get a child");
		return UIElement();
	}
}

UIElement UIElement::getChildAtIndex(int index)
{
	checkNullAndBreak(element, UIElement());
	try {
		UIElement child = UIElement(element->getChildAtIdx(index));
		return child;
	}
	catch (std::exception exception) {
		LOG_ERROR("UI ELEMENT", "Error trying to get a child");
		return UIElement();
	}
}

UIElement UIElement::createChild(const std::string& type, const std::string& name)
{
	checkNullAndBreak(element, UIElement());
	try {
		element->addChild(CEGUI::WindowManager::getSingleton().createWindow(type, name));
		UIElement child = element->getChild(name);;
		return child;
	}
	catch (std::exception exception) {
		LOG_ERROR("UI ELEMENT", "Error trying to create a child");
		return UIElement();
	}	
}

int UIElement::getChildCount()
{
	checkNullAndBreak(element, 0);
	return element->getChildCount();
}

void UIElement::setVisible(bool visible)
{
	checkNullAndBreak(element);
	element->setVisible(visible);
}

bool UIElement::isVisible()
{
	checkNullAndBreak(element, false);
	return element->isVisible();
}

void UIElement::setAlwaysOnTop(bool top)
{
	checkNullAndBreak(element);
	element->setAlwaysOnTop(true);
}

void UIElement::setEnabled(bool enable)
{
	checkNullAndBreak(element);

	if (!enable)
		element->disable();
	else
		element->enable();
}

void UIElement::flipHorizontal()
{
	checkNullAndBreak(element);
	element->getGeometryBuffer().setPivot(CEGUI::Vector3f(element->getPixelSize().d_width / 2.0f, 0.0f, 0.0f));
	element->getGeometryBuffer().setRotation( CEGUI::Quaternion::eulerAnglesDegrees(0.0f, 180.0f, 0.0f));
}

void UIElement::setInheritsAlpha(bool inherits)
{
	checkNullAndBreak(element);
	element->setInheritsAlpha(inherits);
}

void UIElement::setAlpha(float alpha)
{
	checkNullAndBreak(element);
	element->setAlpha(alpha);
}

float UIElement::getScrollPositionScrollBar()
{
	checkNullAndBreak(element, -1);
	if (element->getWindowRendererName() == "Core/Scrollbar") {
		CEGUI::Scrollbar* x = static_cast<CEGUI::Scrollbar*>(element);
		return x->getScrollPosition();
	}
	else
		return -1;
	
	
}

float UIElement::setScrollPositionScrollBar(float value)
{
	checkNullAndBreak(element, -1);
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
	checkNullAndBreak(element, -1);
	if (element->getWindowRendererName() == "Core/ToggleButton") {
		CEGUI::ToggleButton* x = static_cast<CEGUI::ToggleButton*>(element);
	//	x->setPushedState(value);
		x->setSelected(value);
		return 0;
	}
	else
		return -1;
}
