#include "Animation.h"
#include <cmath>

Animation::Animation()
{
    m_Current = 0.f;
    m_Target = 0.f;
    m_Speed = 8.f;
}

void Animation::Set(float value)
{
    m_Current = value;
    m_Target = value;
}

void Animation::Animate(float target, float speed)
{
    m_Target = target;
    m_Speed = speed;
}

void Animation::Update(float dt)
{
    float delta = m_Target - m_Current;

    if (std::abs(delta) < 0.001f)
    {
        m_Current = m_Target;
        return;
    }

    m_Current += delta * m_Speed * dt;
}

float Animation::Value() const
{
    return m_Current;
}

bool Animation::Finished() const
{
    return std::abs(m_Target - m_Current) < 0.001f;
}