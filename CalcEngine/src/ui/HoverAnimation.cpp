#include "HoverAnimation.h"

HoverAnimation::HoverAnimation()
{
    m_Hovered = false;

    m_Scale.Set(1.f);
    m_Glow.Set(0.f);
}

void HoverAnimation::SetHovered(bool hovered)
{
    if (hovered == m_Hovered)
        return;

    m_Hovered = hovered;

    if (hovered)
    {
        m_Scale.Animate(1.05f, 10.f);
        m_Glow.Animate(1.f, 8.f);
    }
    else
    {
        m_Scale.Animate(1.f, 10.f);
        m_Glow.Animate(0.f, 8.f);
    }
}

void HoverAnimation::Update(float dt)
{
    m_Scale.Update(dt);
    m_Glow.Update(dt);
}

float HoverAnimation::Scale() const
{
    return m_Scale.Value();
}

float HoverAnimation::Glow() const
{
    return m_Glow.Value();
}