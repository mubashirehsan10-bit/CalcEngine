#include "Timer.h"

Timer::Timer()
{
    m_Time = 0.f;
    m_Remaining = 0.f;
    m_Running = false;
}

void Timer::Start(float seconds)
{
    m_Time = seconds;
    m_Remaining = seconds;
    m_Running = true;
}

void Timer::Update(float dt)
{
    if (!m_Running)
        return;

    m_Remaining -= dt;

    if (m_Remaining <= 0.f)
    {
        m_Remaining = 0.f;
        m_Running = false;
    }
}

bool Timer::Finished() const
{
    return !m_Running;
}

float Timer::Remaining() const
{
    return m_Remaining;
}