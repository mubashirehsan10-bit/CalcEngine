#pragma once

#include <SFML/Graphics.hpp>

namespace Renderer
{
    //=========================================================
    // Initialization
    //=========================================================

    void Init();

    //=========================================================
    // Basic Shapes
    //=========================================================

    void DrawRectangle(
        sf::RenderTarget& target,
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color fill
    );

    void DrawBorder(
        sf::RenderTarget& target,
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color color,
        float thickness = 1.f
    );

    void DrawShadow(
        sf::RenderTarget& target,
        sf::Vector2f position,
        sf::Vector2f size,
        float radius = 8.f,
        sf::Color color = sf::Color(0, 0, 0, 80)
    );

    //=========================================================
    // Panels
    //=========================================================

    void DrawPanel(
        sf::RenderTarget& target,
        sf::Vector2f position,
        sf::Vector2f size
    );

    void DrawCard(
        sf::RenderTarget& target,
        sf::Vector2f position,
        sf::Vector2f size
    );

    //=========================================================
    // Text
    //=========================================================

    void DrawText(

        sf::RenderTarget& target,

        const sf::Font& font,

        const std::string& text,

        unsigned size,

        sf::Vector2f position,

        sf::Color color,

        bool bold = false

    );

    //=========================================================
    // Lines
    //=========================================================

    void DrawLine(

        sf::RenderTarget& target,

        sf::Vector2f a,

        sf::Vector2f b,

        sf::Color color,

        float thickness = 1.f

    );

    //=========================================================
    // Grid
    //=========================================================

    void DrawGrid(

        sf::RenderTarget& target,

        sf::FloatRect area,

        float spacing,

        sf::Color color

    );

}