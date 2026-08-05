#include "ClickAnimation.h"

ClickAnimation::ClickAnimation()
{
    m_Scale.Set(1.f);
}

void ClickAnimation::Press()
{
    m_Scale.Animate(0.95f, 20.f);
}

void ClickAnimation::Release()
{
    m_Scale.Animate(1.f, 18.f);
}

void ClickAnimation::Update(float dt)
{
    m_Scale.Update(dt);
}

float ClickAnimation::Scale() const
{
    return m_Scale.Value();
}