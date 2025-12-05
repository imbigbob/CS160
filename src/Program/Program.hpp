#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "../StateStack/StateStack.hpp"

class Program {
   public:
    Program();
    ~Program();

    void run();

   private:
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);

    sf::RenderWindow mWindow;

    StateStack mStateStack;

    TextureHolder mTextureHolder;
    FontHolder mFontHolder;

    // PlayerSettings mPlayerSettings1;
    // PlayerSettings mPlayerSettings2;
    // PowerUpSettings mPowerUpSettings1;
    // PowerUpSettings mPowerUpSettings2;

    // HighScore mHighScore;

    void saveSettings() const;

    void loadTextures();
    void loadFonts();
    void loadSettings();

    void registerStates();

    void handleEvent(sf::Event& event);
    void update();
    void draw();
};

#endif
