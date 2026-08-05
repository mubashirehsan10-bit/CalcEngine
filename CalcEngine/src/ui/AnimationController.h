#pragma once

#include <vector>
#include "Animation.h"

class AnimationController
{
public:

    void Add(Animation* animation);

    void Remove(Animation* animation);

    void Update(float dt);

    void Clear();

private:

    std::vector<Animation*> m_Animations;

};