#include "SlideAnimation.h"

SlideAnimation::SlideAnimation()
{
    m_X.Set(0.f);
    m_Y.Set(0.f);
}

void SlideAnimation::SetPosition(float x, float y)
{
    m_X.Set(x);
    m_Y.Set(y);
}

void SlideAnimation::SlideTo(float x, float y)
{
    m_X.Animate(x, 10.f);
    m_Y.Animate(y, 10.f);
}

void SlideAnimation::Update(float dt)
{
    m_X.Update(dt);
    m_Y.Update(dt);
}

float SlideAnimation::X() const
{
    return m_X.Value();
}

float SlideAnimation::Y() const
{
    return m_Y.Value();
}