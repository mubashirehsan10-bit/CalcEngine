#include "PulseAnimation.h"
#include <cmath>

PulseAnimation::PulseAnimation()
{
    m_Time = 0.f;
}

void PulseAnimation::Update(float dt)
{
    m_Time += dt;
}

float PulseAnimation::Value() const
{
    return 1.f + 0.03f * std::sin(m_Time * 4.f);
}