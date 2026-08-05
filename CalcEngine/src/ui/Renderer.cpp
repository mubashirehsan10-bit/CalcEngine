#include "Renderer.h"
#include "Theme.h"

namespace Renderer
{

    void Init()
    {

    }

    void DrawRectangle(

        sf::RenderTarget& target,

        sf::Vector2f position,

        sf::Vector2f size,

        sf::Color fill

    )
    {
        sf::RectangleShape rect;

        rect.setPosition(position);

        rect.setSize(size);

        rect.setFillColor(fill);

        target.draw(rect);
    }

    void DrawBorder(

        sf::RenderTarget& target,

        sf::Vector2f position,

        sf::Vector2f size,

        sf::Color color,

        float thickness

    )
    {
        sf::RectangleShape rect;

        rect.setPosition(position);

        rect.setSize(size);

        rect.setFillColor(sf::Color::Transparent);

        rect.setOutlineThickness(thickness);

        rect.setOutlineColor(color);

        target.draw(rect);
    }

    void DrawShadow(

        sf::RenderTarget& target,

        sf::Vector2f position,

        sf::Vector2f size,

        float radius,

        sf::Color color

    )
    {
        sf::RectangleShape shadow;

        shadow.setPosition({

            position.x + radius,

            position.y + radius

            });

        shadow.setSize(size);

        shadow.setFillColor(color);

        target.draw(shadow);
    }
    void DrawPanel(

        sf::RenderTarget& target,

        sf::Vector2f position,

        sf::Vector2f size

    )
    {
        DrawShadow(

            target,

            position,

            size,

            Theme::SHADOW_OFFSET,

            Theme::Shadow

        );

        DrawRectangle(

            target,

            position,

            size,

            Theme::Surface

        );

        DrawBorder(

            target,

            position,

            size,

            Theme::Border,

            1.f

        );
    }

    void DrawCard(

        sf::RenderTarget& target,

        sf::Vector2f position,

        sf::Vector2f size

    )
    {
        DrawShadow(

            target,

            position,

            size,

            Theme::SHADOW_OFFSET + 2.f,

            Theme::Shadow

        );

        DrawRectangle(

            target,

            position,

            size,

            Theme::Surface2

        );

        DrawBorder(

            target,

            position,

            size,

            Theme::Border,

            1.f

        );
    }

    void DrawText(

        sf::RenderTarget& target,

        const sf::Font& font,

        const std::string& text,

        unsigned size,

        sf::Vector2f position,

        sf::Color color,

        bool bold

    )
    {
        sf::Text txt(font);

        txt.setString(text);

        txt.setCharacterSize(size);

        txt.setPosition(position);

        txt.setFillColor(color);

        if (bold)
            txt.setStyle(sf::Text::Bold);

        target.draw(txt);
    }
    void DrawLine(

        sf::RenderTarget& target,

        sf::Vector2f start,

        sf::Vector2f end,

        sf::Color color,

        float thickness

    )
    {
        sf::Vector2f direction = end - start;

        float length = std::sqrt(direction.x * direction.x +
            direction.y * direction.y);

        if (length <= 0.001f)
            return;

        sf::RectangleShape line;

        line.setSize({ length, thickness });

        line.setOrigin({ 0.f, thickness / 2.f });

        line.setPosition(start);

        line.setRotation(

            sf::degrees(

                std::atan2(direction.y, direction.x) *

                180.f /

                3.14159265358979323846f

            )

        );

        line.setFillColor(color);

        target.draw(line);
    }

    void DrawGrid(

        sf::RenderTarget& target,

        sf::FloatRect area,

        float spacing,

        sf::Color color

    )
    {
        for (float x = area.position.x;
            x <= area.position.x + area.size.x;
            x += spacing)
        {
            DrawLine(

                target,

                { x, area.position.y },

                { x, area.position.y + area.size.y },

                color

            );
        }

        for (float y = area.position.y;
            y <= area.position.y + area.size.y;
            y += spacing)
        {
            DrawLine(

                target,

                { area.position.x, y },

                { area.position.x + area.size.x, y },

                color

            );
        }
    }
    //=========================================================
// Fill Area
//=========================================================

    void Fill(

        sf::RenderTarget& target,

        sf::FloatRect area,

        sf::Color color

    )
    {
        sf::RectangleShape rect;

        rect.setPosition(area.position);

        rect.setSize(area.size);

        rect.setFillColor(color);

        target.draw(rect);
    }

    //=========================================================
    // Divider
    //=========================================================

    void DrawDivider(

        sf::RenderTarget& target,

        float y,

        float width,

        sf::Color color

    )
    {
        DrawLine(

            target,

            { 0.f, y },

            { width, y },

            color

        );
    }

    //=========================================================
    // Circle
    //=========================================================

    void DrawCircle(

        sf::RenderTarget& target,

        sf::Vector2f center,

        float radius,

        sf::Color fill

    )
    {
        sf::CircleShape circle(radius);

        circle.setOrigin({ radius, radius });

        circle.setPosition(center);

        circle.setFillColor(fill);

        target.draw(circle);
    }

    //=========================================================
    // Circle Border
    //=========================================================

    void DrawCircleBorder(

        sf::RenderTarget& target,

        sf::Vector2f center,

        float radius,

        sf::Color color,

        float thickness

    )
    {
        sf::CircleShape circle(radius);

        circle.setOrigin({ radius, radius });

        circle.setPosition(center);

        circle.setFillColor(sf::Color::Transparent);

        circle.setOutlineThickness(thickness);

        circle.setOutlineColor(color);

        target.draw(circle);
    }

    //=========================================================
    // End Namespace
    //=========================================================

}