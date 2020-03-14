#include "InterfaceSystem.h"

#include <CEGUI/Event.h>

InterfaceSystem::InterfaceSystem()
{

}

InterfaceSystem::~InterfaceSystem()
{
	
}

void InterfaceSystem::close()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::System::getSingleton().destroy();

	CEGUI::OgreRenderer::destroy(*mRenderer);
	destroy();
}

void InterfaceSystem::setupResources()
{
	CEGUI::DefaultResourceProvider* rp =
		static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	// setup resources
	rp->setResourceGroupDirectory("schemes", "./Assets/UI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "./Assets/UI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "./Assets/UI/fonts/");
	rp->setResourceGroupDirectory("layouts", "./Assets/UI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "./Assets/UI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "./Assets/UI/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "./Assets/UI/xml_schemas/");
	rp->setResourceGroupDirectory("animations", "./Assets/UI/animations/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");

	// load themes
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	// load fonts
	CEGUI::FontManager::getSingleton().createFreeTypeFont("Batang", 12, true, "batang.ttf", "fonts");
}

UIElement* InterfaceSystem::getRoot()
{
	return root;
}

void InterfaceSystem::createRoot()
{
	root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

void InterfaceSystem::init(Window* window)
{
	// init
	mRenderer = &CEGUI::OgreRenderer::create(*window->getRenderWindow());
	CEGUI::System::create(*mRenderer);

	// esto deberia ir en el ResourceManager!!
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
}

void InterfaceSystem::render()
{
	mRenderer->beginRendering();
	CEGUI::System::getSingleton().renderAllGUIContexts();
	mRenderer->endRendering();
}

void InterfaceSystem::update(float deltaTime)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(deltaTime);
}

UIElement* InterfaceSystem::loadLayout(const std::string& filename)
{
	try {
		return CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
	}
	catch (std::exception e) {
		printf("INTERFACE SYSTEM: trying to load \"%s\" layout\n", filename.c_str());
		return nullptr;
	}
}

CEGUI::Key::Scan InterfaceSystem::SDLKeyToCEGUIKey(int key)
{
    using namespace CEGUI;
    switch (key)
    {
    case SDLK_BACKSPACE:    return Key::Backspace;
    case SDLK_TAB:          return Key::Tab;
    case SDLK_RETURN:       return Key::Return;
    case SDLK_PAUSE:        return Key::Pause;
    case SDLK_ESCAPE:       return Key::Escape;
    case SDLK_SPACE:        return Key::Space;
    case SDLK_COMMA:        return Key::Comma;
    case SDLK_MINUS:        return Key::Minus;
    case SDLK_PERIOD:       return Key::Period;
    case SDLK_SLASH:        return Key::Slash;
    case SDLK_0:            return Key::Zero;
    case SDLK_1:            return Key::One;
    case SDLK_2:            return Key::Two;
    case SDLK_3:            return Key::Three;
    case SDLK_4:            return Key::Four;
    case SDLK_5:            return Key::Five;
    case SDLK_6:            return Key::Six;
    case SDLK_7:            return Key::Seven;
    case SDLK_8:            return Key::Eight;
    case SDLK_9:            return Key::Nine;
    case SDLK_COLON:        return Key::Colon;
    case SDLK_SEMICOLON:    return Key::Semicolon;
    case SDLK_EQUALS:       return Key::Equals;
    case SDLK_LEFTBRACKET:  return Key::LeftBracket;
    case SDLK_BACKSLASH:    return Key::Backslash;
    case SDLK_RIGHTBRACKET: return Key::RightBracket;
    case SDLK_a:            return Key::A;
    case SDLK_b:            return Key::B;
    case SDLK_c:            return Key::C;
    case SDLK_d:            return Key::D;
    case SDLK_e:            return Key::E;
    case SDLK_f:            return Key::F;
    case SDLK_g:            return Key::G;
    case SDLK_h:            return Key::H;
    case SDLK_i:            return Key::I;
    case SDLK_j:            return Key::J;
    case SDLK_k:            return Key::K;
    case SDLK_l:            return Key::L;
    case SDLK_m:            return Key::M;
    case SDLK_n:            return Key::N;
    case SDLK_o:            return Key::O;
    case SDLK_p:            return Key::P;
    case SDLK_q:            return Key::Q;
    case SDLK_r:            return Key::R;
    case SDLK_s:            return Key::S;
    case SDLK_t:            return Key::T;
    case SDLK_u:            return Key::U;
    case SDLK_v:            return Key::V;
    case SDLK_w:            return Key::W;
    case SDLK_x:            return Key::X;
    case SDLK_y:            return Key::Y;
    case SDLK_z:            return Key::Z;
    case SDLK_DELETE:       return Key::Delete;
    case SDLK_KP_0:         return Key::Numpad0;
    case SDLK_KP_1:         return Key::Numpad1;
    case SDLK_KP_2:         return Key::Numpad2;
    case SDLK_KP_3:         return Key::Numpad3;
    case SDLK_KP_4:         return Key::Numpad4;
    case SDLK_KP_5:         return Key::Numpad5;
    case SDLK_KP_6:         return Key::Numpad6;
    case SDLK_KP_7:         return Key::Numpad7;
    case SDLK_KP_8:         return Key::Numpad8;
    case SDLK_KP_9:         return Key::Numpad9;
    case SDLK_KP_PERIOD:    return Key::Decimal;
    case SDLK_KP_DIVIDE:    return Key::Divide;
    case SDLK_KP_MULTIPLY:  return Key::Multiply;
    case SDLK_KP_MINUS:     return Key::Subtract;
    case SDLK_KP_PLUS:      return Key::Add;
    case SDLK_KP_ENTER:     return Key::NumpadEnter;
    case SDLK_KP_EQUALS:    return Key::NumpadEquals;
    case SDLK_UP:           return Key::ArrowUp;
    case SDLK_DOWN:         return Key::ArrowDown;
    case SDLK_RIGHT:        return Key::ArrowRight;
    case SDLK_LEFT:         return Key::ArrowLeft;
    case SDLK_INSERT:       return Key::Insert;
    case SDLK_HOME:         return Key::Home;
    case SDLK_END:          return Key::End;
    case SDLK_PAGEUP:       return Key::PageUp;
    case SDLK_PAGEDOWN:     return Key::PageDown;
    case SDLK_F1:           return Key::F1;
    case SDLK_F2:           return Key::F2;
    case SDLK_F3:           return Key::F3;
    case SDLK_F4:           return Key::F4;
    case SDLK_F5:           return Key::F5;
    case SDLK_F6:           return Key::F6;
    case SDLK_F7:           return Key::F7;
    case SDLK_F8:           return Key::F8;
    case SDLK_F9:           return Key::F9;
    case SDLK_F10:          return Key::F10;
    case SDLK_F11:          return Key::F11;
    case SDLK_F12:          return Key::F12;
    case SDLK_F13:          return Key::F13;
    case SDLK_F14:          return Key::F14;
    case SDLK_F15:          return Key::F15;
    case SDLK_NUMLOCKCLEAR: return Key::NumLock;
    case SDLK_SCROLLLOCK:   return Key::ScrollLock;
    case SDLK_RSHIFT:       return Key::RightShift;
    case SDLK_LSHIFT:       return Key::LeftShift;
    case SDLK_RCTRL:        return Key::RightControl;
    case SDLK_LCTRL:        return Key::LeftControl;
    case SDLK_RALT:         return Key::RightAlt;
    case SDLK_LALT:         return Key::LeftAlt;
    case SDLK_LGUI:         return Key::LeftWindows;
    case SDLK_RGUI:         return Key::RightWindows;
    case SDLK_SYSREQ:       return Key::SysRq;
    case SDLK_MENU:         return Key::AppMenu;
    case SDLK_POWER:        return Key::Power;
    default:                return Key::Unknown;
    }
    return Key::Unknown;
}
