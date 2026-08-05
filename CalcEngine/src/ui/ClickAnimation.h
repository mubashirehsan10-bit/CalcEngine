#pragma once

#include "Animation.h"

class ClickAnimation
{
public:

    ClickAnimation();

    void Press();

    void Release();

    void Update(float dt);

    float Scale() const;

private:

    Animation m_Scale;
};