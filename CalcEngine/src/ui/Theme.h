#pragma once
#include <SFML/Graphics.hpp>

namespace Theme
{
    //=========================
    // Window
    //=========================
    constexpr unsigned WINDOW_WIDTH = 1400;
    constexpr unsigned WINDOW_HEIGHT = 850;

    constexpr float PADDING = 20.f;
    constexpr float BORDER_RADIUS = 12.f;
    constexpr float SHADOW_OFFSET = 5.f;

    //=========================
    // Colors
    //=========================

    extern const sf::Color Background;

    extern const sf::Color Surface;
    extern const sf::Color Surface2;
    extern const sf::Color Surface3;

    extern const sf::Color Primary;
    extern const sf::Color PrimaryHover;
    extern const sf::Color PrimaryDark;

    extern const sf::Color Secondary;

    extern const sf::Color Success;
    extern const sf::Color Warning;
    extern const sf::Color Error;

    extern const sf::Color White;
    extern const sf::Color Gray;
    extern const sf::Color DarkGray;

    extern const sf::Color Border;

    extern const sf::Color Shadow;

    extern const sf::Color GraphGrid;
    extern const sf::Color GraphAxis;

    //=========================
    // Fonts
    //=========================

    extern sf::Font MainFont;

    bool Load();

}