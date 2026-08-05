#include "Ease.h"

float Ease::Linear(float t)
{
    return t;
}

float Ease::Smooth(float t)
{
    return t * t * (3.f - 2.f * t);
}

float Ease::EaseIn(float t)
{
    return t * t;
}

float Ease::EaseOut(float t)
{
    return 1.f - (1.f - t) * (1.f - t);
}

float Ease::EaseInOut(float t)
{
    if (t < 0.5f)
        return 2.f * t * t;

    return 1.f - ((-2.f * t + 2.f) * (-2.f * t + 2.f)) / 2.f;
}