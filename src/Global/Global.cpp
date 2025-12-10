#include "Global.hpp"

#include <SFML/Window.hpp>

#define DESKTOP_MODE sf::VideoMode::getDesktopMode()
#define WINDOW_DESKTOP_RATIO 0.75f

const int Global::WINDOW_WIDTH = DESKTOP_MODE.width * WINDOW_DESKTOP_RATIO;
const int Global::WINDOW_HEIGHT = DESKTOP_MODE.height * WINDOW_DESKTOP_RATIO;
