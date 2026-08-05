#pragma once

#include "Animation.h"

class HoverAnimation
{
public:

    HoverAnimation();

    void SetHovered(bool hovered);

    void Update(float dt);

    float Scale() const;

    float Glow() const;

private:

    Animation m_Scale;
    Animation m_Glow;

    bool m_Hovered;
};