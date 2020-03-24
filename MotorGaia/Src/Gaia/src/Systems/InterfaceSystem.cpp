#include "InterfaceSystem.h"
#include "DebugUtils.h"

#include <CEGUI/Event.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

std::map<std::string, UIEvent> InterfaceSystem::events;

InterfaceSystem::InterfaceSystem() : renderer(nullptr), root(nullptr), deltaX(0), deltaY(0)
#ifdef _DEBUG
, fpsText(nullptr)
#endif // _DEBUG
{

}

InterfaceSystem::~InterfaceSystem()
{

}

void InterfaceSystem::close()
{
    renderer->destroySystem();
	destroy();
}

void InterfaceSystem::setupResources()
{
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}

UIElement* InterfaceSystem::getRoot()
{
	return root;
}

void InterfaceSystem::createRoot()
{
	root = new UIElement(CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root"));
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root->getElement());
}

void InterfaceSystem::init(Window* window)
{
	// init
    CEGUI::OgreRenderer& ogreRenderer = CEGUI::OgreRenderer::bootstrapSystem(*window->getRenderWindow());
    renderer = &ogreRenderer;

	setupResources();
	createRoot();

	// Callback definitions
	onKeyDown([this](std::string keyName, int key) { CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(SDLKeyToCEGUIKey(key)); });
	onKeyUp([this](std::string keyName, int key) { CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(SDLKeyToCEGUIKey(key)); });

    onTextInput([this](std::string text) { for(char c : text) CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(c); });
    onMouseMotion([this](int x, int y) { CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y); });

    onMouseLeftButtonDown([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton); });
    onMouseLeftButtonUp([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton); });

    onMouseRightButtonDown([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton); });
    onMouseRightButtonUp([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::RightButton); });

    onMouseMiddleButtonDown([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton); });
    onMouseMiddleButtonUp([this]() { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton); });

    onMouseWheelScrollY([this](int value) { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(value); });

    onWindowResized([this](int width, int height) { CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(width, height)); });

    onControllerButtonDown([this](int index, int button) { processControllerButtonDown(index, button); });
    onControllerButtonUp([this](int index, int button) { processControllerButtonUp(index, button); });
}

void InterfaceSystem::render()
{
    /* Ogre does it for us */

	//renderer->beginRendering();
	//CEGUI::System::getSingleton().renderAllGUIContexts();
	//renderer->endRendering();
}

void InterfaceSystem::update(float deltaTime)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(deltaTime);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(deltaX, deltaY);

#ifdef _DEBUG
	fpsText->setText("FPS: " + std::to_string((int)(1.0 / deltaTime)));
#endif
}

UIElement* InterfaceSystem::loadLayout(const std::string& filename)
{
	try {
        return new UIElement(CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename));
	}
	catch (std::exception e) {
		LOG_ERROR("INTERFACE SYSTEM","trying to load \"%s\" layout", filename.c_str());
		return nullptr;
	}
}

void InterfaceSystem::initDefaultResources()
{
    /* Resources are loaded at this moment by OgreResourceProvider */
    /* Here we just initilialize default interface resources */
    // load themes
    CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // load fonts
    CEGUI::FontManager::getSingleton().createFreeTypeFont("Batang", 16, true, "batang.ttf");


#ifdef _DEBUG
    fpsText = new UIElement(root->getElement()->createChild("TaharezLook/StaticText", "FPSText"));
    fpsText->setPosition(0.9f, 0.0f);
    fpsText->setSize(0.1f,0.1f);
#endif
}

CEGUI::Key::Scan InterfaceSystem::SDLKeyToCEGUIKey(int key)
{
    switch (key)
    {
    case SDLK_BACKSPACE:    return CEGUI::Key::Backspace;
    case SDLK_TAB:          return CEGUI::Key::Tab;
    case SDLK_RETURN:       return CEGUI::Key::Return;
    case SDLK_PAUSE:        return CEGUI::Key::Pause;
    case SDLK_ESCAPE:       return CEGUI::Key::Escape;
    case SDLK_SPACE:        return CEGUI::Key::Space;
    case SDLK_COMMA:        return CEGUI::Key::Comma;
    case SDLK_MINUS:        return CEGUI::Key::Minus;
    case SDLK_PERIOD:       return CEGUI::Key::Period;
    case SDLK_SLASH:        return CEGUI::Key::Slash;
    case SDLK_0:            return CEGUI::Key::Zero;
    case SDLK_1:            return CEGUI::Key::One;
    case SDLK_2:            return CEGUI::Key::Two;
    case SDLK_3:            return CEGUI::Key::Three;
    case SDLK_4:            return CEGUI::Key::Four;
    case SDLK_5:            return CEGUI::Key::Five;
    case SDLK_6:            return CEGUI::Key::Six;
    case SDLK_7:            return CEGUI::Key::Seven;
    case SDLK_8:            return CEGUI::Key::Eight;
    case SDLK_9:            return CEGUI::Key::Nine;
    case SDLK_COLON:        return CEGUI::Key::Colon;
    case SDLK_SEMICOLON:    return CEGUI::Key::Semicolon;
    case SDLK_EQUALS:       return CEGUI::Key::Equals;
    case SDLK_LEFTBRACKET:  return CEGUI::Key::LeftBracket;
    case SDLK_BACKSLASH:    return CEGUI::Key::Backslash;
    case SDLK_RIGHTBRACKET: return CEGUI::Key::RightBracket;
    case SDLK_a:            return CEGUI::Key::A;
    case SDLK_b:            return CEGUI::Key::B;
    case SDLK_c:            return CEGUI::Key::C;
    case SDLK_d:            return CEGUI::Key::D;
    case SDLK_e:            return CEGUI::Key::E;
    case SDLK_f:            return CEGUI::Key::F;
    case SDLK_g:            return CEGUI::Key::G;
    case SDLK_h:            return CEGUI::Key::H;
    case SDLK_i:            return CEGUI::Key::I;
    case SDLK_j:            return CEGUI::Key::J;
    case SDLK_k:            return CEGUI::Key::K;
    case SDLK_l:            return CEGUI::Key::L;
    case SDLK_m:            return CEGUI::Key::M;
    case SDLK_n:            return CEGUI::Key::N;
    case SDLK_o:            return CEGUI::Key::O;
    case SDLK_p:            return CEGUI::Key::P;
    case SDLK_q:            return CEGUI::Key::Q;
    case SDLK_r:            return CEGUI::Key::R;
    case SDLK_s:            return CEGUI::Key::S;
    case SDLK_t:            return CEGUI::Key::T;
    case SDLK_u:            return CEGUI::Key::U;
    case SDLK_v:            return CEGUI::Key::V;
    case SDLK_w:            return CEGUI::Key::W;
    case SDLK_x:            return CEGUI::Key::X;
    case SDLK_y:            return CEGUI::Key::Y;
    case SDLK_z:            return CEGUI::Key::Z;
    case SDLK_DELETE:       return CEGUI::Key::Delete;
    case SDLK_KP_0:         return CEGUI::Key::Numpad0;
    case SDLK_KP_1:         return CEGUI::Key::Numpad1;
    case SDLK_KP_2:         return CEGUI::Key::Numpad2;
    case SDLK_KP_3:         return CEGUI::Key::Numpad3;
    case SDLK_KP_4:         return CEGUI::Key::Numpad4;
    case SDLK_KP_5:         return CEGUI::Key::Numpad5;
    case SDLK_KP_6:         return CEGUI::Key::Numpad6;
    case SDLK_KP_7:         return CEGUI::Key::Numpad7;
    case SDLK_KP_8:         return CEGUI::Key::Numpad8;
    case SDLK_KP_9:         return CEGUI::Key::Numpad9;
    case SDLK_KP_PERIOD:    return CEGUI::Key::Decimal;
    case SDLK_KP_DIVIDE:    return CEGUI::Key::Divide;
    case SDLK_KP_MULTIPLY:  return CEGUI::Key::Multiply;
    case SDLK_KP_MINUS:     return CEGUI::Key::Subtract;
    case SDLK_KP_PLUS:      return CEGUI::Key::Add;
    case SDLK_KP_ENTER:     return CEGUI::Key::NumpadEnter;
    case SDLK_KP_EQUALS:    return CEGUI::Key::NumpadEquals;
    case SDLK_UP:           return CEGUI::Key::ArrowUp;
    case SDLK_DOWN:         return CEGUI::Key::ArrowDown;
    case SDLK_RIGHT:        return CEGUI::Key::ArrowRight;
    case SDLK_LEFT:         return CEGUI::Key::ArrowLeft;
    case SDLK_INSERT:       return CEGUI::Key::Insert;
    case SDLK_HOME:         return CEGUI::Key::Home;
    case SDLK_END:          return CEGUI::Key::End;
    case SDLK_PAGEUP:       return CEGUI::Key::PageUp;
    case SDLK_PAGEDOWN:     return CEGUI::Key::PageDown;
    case SDLK_F1:           return CEGUI::Key::F1;
    case SDLK_F2:           return CEGUI::Key::F2;
    case SDLK_F3:           return CEGUI::Key::F3;
    case SDLK_F4:           return CEGUI::Key::F4;
    case SDLK_F5:           return CEGUI::Key::F5;
    case SDLK_F6:           return CEGUI::Key::F6;
    case SDLK_F7:           return CEGUI::Key::F7;
    case SDLK_F8:           return CEGUI::Key::F8;
    case SDLK_F9:           return CEGUI::Key::F9;
    case SDLK_F10:          return CEGUI::Key::F10;
    case SDLK_F11:          return CEGUI::Key::F11;
    case SDLK_F12:          return CEGUI::Key::F12;
    case SDLK_F13:          return CEGUI::Key::F13;
    case SDLK_F14:          return CEGUI::Key::F14;
    case SDLK_F15:          return CEGUI::Key::F15;
    case SDLK_NUMLOCKCLEAR: return CEGUI::Key::NumLock;
    case SDLK_SCROLLLOCK:   return CEGUI::Key::ScrollLock;
    case SDLK_RSHIFT:       return CEGUI::Key::RightShift;
    case SDLK_LSHIFT:       return CEGUI::Key::LeftShift;
    case SDLK_RCTRL:        return CEGUI::Key::RightControl;
    case SDLK_LCTRL:        return CEGUI::Key::LeftControl;
    case SDLK_RALT:         return CEGUI::Key::RightAlt;
    case SDLK_LALT:         return CEGUI::Key::LeftAlt;
    case SDLK_LGUI:         return CEGUI::Key::LeftWindows;
    case SDLK_RGUI:         return CEGUI::Key::RightWindows;
    case SDLK_SYSREQ:       return CEGUI::Key::SysRq;
    case SDLK_MENU:         return CEGUI::Key::AppMenu;
    case SDLK_POWER:        return CEGUI::Key::Power;
    default:                return CEGUI::Key::Unknown;
    }
    return CEGUI::Key::Unknown;
}

void InterfaceSystem::processControllerButtonDown(int index, int button)
{
    if (button == SDL_CONTROLLER_BUTTON_A)
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
}

void InterfaceSystem::processControllerButtonUp(int index, int button)
{
    if (button == SDL_CONTROLLER_BUTTON_A)
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
}

void InterfaceSystem::registerEvent(const std::string& eventName, UIEvent event)
{
	if (events.find(eventName) != events.end())
	{
		LOG("INTERFACE SYSTEM: Error registering event %s\n", eventName.c_str());
		return;
	}

	events[eventName] = event;
}

UIEvent InterfaceSystem::getEvent(const std::string& eventName)
{
	if (events.find(eventName) == events.end())
	{
		LOG("INTERFACE SYSTEM: Event %s not found\n", eventName.c_str());
		return UIEvent("",nullptr);
	}

	return events[eventName];
}

