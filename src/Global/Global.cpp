#include "Global.hpp"

#include <SFML/Window.hpp>

#define DESKTOP_MODE sf::VideoMode::getDesktopMode()
#define WINDOW_DESKTOP_RATIO 0.75f

float Global::SPEED_MODIFIER = 1.f;
float Global::DIFFICULTY_MODIFIER = 1.f;

int Global::SCORE = 0;

const int Global::WINDOW_WIDTH = DESKTOP_MODE.width * WINDOW_DESKTOP_RATIO;
const int Global::WINDOW_HEIGHT = DESKTOP_MODE.height * WINDOW_DESKTOP_RATIO;

const int Global::TILE_SIZE = 90;
const int Global::NUM_TILES_X = Global::WINDOW_WIDTH / Global::TILE_SIZE;
const int Global::NUM_TILES_Y = Global::WINDOW_HEIGHT / Global::TILE_SIZE;

void Global::setDefault() {
    SPEED_MODIFIER = 1.f;
    DIFFICULTY_MODIFIER = 1.f;
    SCORE = 0;
}

void Global::save(std::ofstream& fout) {
    fout << SPEED_MODIFIER << ' ' << DIFFICULTY_MODIFIER << ' ' << SCORE
         << '\n';
}

void Global::load(std::ifstream& fin) {
    fin >> SPEED_MODIFIER >> DIFFICULTY_MODIFIER >> SCORE;
}
