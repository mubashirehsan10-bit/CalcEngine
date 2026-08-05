#pragma once

#include "Animation.h"

class SlideAnimation
{
public:

    SlideAnimation();

    void SlideTo(float x, float y);

    void SetPosition(float x, float y);

    void Update(float dt);

    float X() const;

    float Y() const;

private:

    Animation m_X;

    Animation m_Y;
};