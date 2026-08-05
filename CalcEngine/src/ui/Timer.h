#pragma once

class Timer
{
public:

    Timer();

    void Start(float seconds);

    void Update(float dt);

    bool Finished() const;

    float Remaining() const;

private:

    float m_Time;

    float m_Remaining;

    bool m_Running;

};