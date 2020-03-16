#include "WindowEventListener.h"


WindowEventListener::WindowEventListener() : EventListener()
{
}

WindowEventListener::~WindowEventListener()
{
}

void WindowEventListener::processEvent(const SDL_Event& event)
{
    if (event.type == SDL_WINDOWEVENT) {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            if (windowShown) windowShown();
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            if (windowHidden) windowHidden();
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            if (windowExposed) windowExposed();
            break;
        case SDL_WINDOWEVENT_MOVED:
            if (windowMoved) windowMoved(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            if (windowResized) windowResized(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            if (windowSizeChanged) windowSizeChanged(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            if (windowMinimized) windowMinimized();
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            if (windowMaximized) windowMaximized();
            break;
        case SDL_WINDOWEVENT_RESTORED:
            if (windowRestored) windowRestored();
            break;
        case SDL_WINDOWEVENT_ENTER:
            if (windowMouseEnter) windowMouseEnter();
            break;
        case SDL_WINDOWEVENT_LEAVE:
            if (windowMouseLeave) windowMouseLeave();
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            if (windowKeyboardFocusGained) windowKeyboardFocusGained();
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            if (windowKeyboardFocusLost) windowKeyboardFocusLost();
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (windowClose) windowClose();
            break;
// NOT NEEDED AT THE MOMENT
/*#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            SDL_Log("Window %d is offered a focus", event.window.windowID);
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            SDL_Log("Window %d has a special hit test", event.window.windowID);
            break;
#endif*/
        default:
            break;
        }
    }
}

void WindowEventListener::onWindowShown(std::function<void()> callback)
{
    windowShown = callback;
}

void WindowEventListener::onWindowHidden(std::function<void()> callback)
{
    windowHidden = callback;
}

void WindowEventListener::onWindowExposed(std::function<void()> callback)
{
    windowExposed = callback;
}

void WindowEventListener::onWindowMoved(std::function<void(int x, int y)> callback)
{
    windowMoved = callback;
}

void WindowEventListener::onWindowResized(std::function<void(unsigned int width, unsigned int height)> callback)
{
    windowResized = callback;
}

void WindowEventListener::onWindowSizeChanged(std::function<void(unsigned int width, unsigned int height)> callback)
{
    windowSizeChanged = callback;
}

void WindowEventListener::onWindowMinimized(std::function<void()> callback)
{
    windowMaximized = callback;
}

void WindowEventListener::onWindowMaximized(std::function<void()> callback)
{
    windowMinimized = callback;
}

void WindowEventListener::onWindowRestored(std::function<void()> callback)
{
    windowRestored = callback;
}

void WindowEventListener::onWindowMouseEnter(std::function<void()> callback)
{
    windowMouseEnter = callback;
}

void WindowEventListener::onWindowMouseLeave(std::function<void()> callback)
{
    windowMouseLeave = callback;
}

void WindowEventListener::onWindowKeyboardFocusGained(std::function<void()> callback)
{
    windowKeyboardFocusGained = callback;
}

void WindowEventListener::onWindowKeyboardFocusLost(std::function<void()> callback)
{
    windowKeyboardFocusLost = callback;
}

void WindowEventListener::onWindowClose(std::function<void()> callback)
{
    windowClose = callback;
}
