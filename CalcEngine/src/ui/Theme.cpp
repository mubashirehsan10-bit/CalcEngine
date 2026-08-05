#include "Theme.h"
#include <iostream>

namespace Theme
{

    const sf::Color Background = { 12,16,22 };

    const sf::Color Surface = { 24,30,40 };
    const sf::Color Surface2 = { 30,37,49 };
    const sf::Color Surface3 = { 38,46,60 };

    const sf::Color Primary = { 0,255,170 };
    const sf::Color PrimaryHover = { 50,255,190 };
    const sf::Color PrimaryDark = { 0,180,120 };

    const sf::Color Secondary = { 0,180,255 };

    const sf::Color Success = { 90,255,140 };
    const sf::Color Warning = { 255,215,80 };
    const sf::Color Error = { 255,95,95 };

    const sf::Color White = { 245,245,245 };
    const sf::Color Gray = { 170,180,195 };
    const sf::Color DarkGray = { 110,120,135 };

    const sf::Color Border = { 55,68,85 };

    const sf::Color Shadow = { 0,0,0,90 };

    const sf::Color GraphGrid = { 38,48,60 };
    const sf::Color GraphAxis = { 95,110,130 };

    sf::Font MainFont;

    bool Load()
    {
        if (!MainFont.openFromFile("Assets/Fonts/Inter-Regular.ttf"))
        {
            std::cout << "Unable to load font.\n";
            return false;
        }

        return true;
    }

}