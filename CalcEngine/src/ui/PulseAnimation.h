#pragma once

class PulseAnimation
{
public:

    PulseAnimation();

    void Update(float dt);

    float Value() const;

private:

    float m_Time;
};