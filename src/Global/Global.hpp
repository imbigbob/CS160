#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <fstream>

struct Global {
    static float SPEED_MODIFIER;
    static float DIFFICULTY_MODIFIER;

    static int SCORE;

    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;

    static const int TILE_SIZE;
    static const int NUM_TILES_X;
    static const int NUM_TILES_Y;

    static void setDefault();

    static void save(std::ofstream& fout);
    static void load(std::ifstream& fin);
};

#endif
