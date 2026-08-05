#pragma once

#include "Animation.h"

class FadeAnimation
{
public:

    FadeAnimation();

    void FadeIn();

    void FadeOut();

    void Update(float dt);

    float Alpha() const;

private:

    Animation m_Alpha;
};