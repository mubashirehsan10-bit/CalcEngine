#include "AnimationController.h"
#include <algorithm>

void AnimationController::Add(Animation* animation)
{
    if (animation == nullptr)
        return;

    if (std::find(m_Animations.begin(),
        m_Animations.end(),
        animation) == m_Animations.end())
    {
        m_Animations.push_back(animation);
    }
}

void AnimationController::Remove(Animation* animation)
{
    m_Animations.erase(

        std::remove(

            m_Animations.begin(),

            m_Animations.end(),

            animation

        ),

        m_Animations.end()

    );
}

void AnimationController::Update(float dt)
{
    for (Animation* animation : m_Animations)
    {
        if (animation)
            animation->Update(dt);
    }
}

void AnimationController::Clear()
{
    m_Animations.clear();
}