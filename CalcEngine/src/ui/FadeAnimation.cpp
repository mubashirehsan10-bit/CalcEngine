#include "FadeAnimation.h"

FadeAnimation::FadeAnimation()
{
    m_Alpha.Set(255.f);
}

void FadeAnimation::FadeIn()
{
    m_Alpha.Animate(255.f, 8.f);
}

void FadeAnimation::FadeOut()
{
    m_Alpha.Animate(0.f, 8.f);
}

void FadeAnimation::Update(float dt)
{
    m_Alpha.Update(dt);
}

float FadeAnimation::Alpha() const
{
    return m_Alpha.Value();
}