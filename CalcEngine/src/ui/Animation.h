#pragma once

#include <SFML/System.hpp>

class Animation
{
public:

    Animation();

    void Set(float value);

    void Animate(float target, float speed);

    void Update(float dt);

    float Value() const;

    bool Finished() const;

private:

    float m_Current;

    float m_Target;

    float m_Speed;
};