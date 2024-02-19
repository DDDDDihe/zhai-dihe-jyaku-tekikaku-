#include "EmitterComponent.h"
#include "ParticleSystem.h"

EmitterComponent::EmitterComponent(class Entity* owner)
    : Component(owner)
    , mDuration(10.0f)
    , mEmitNumOnce(100)
    , mMaxParticles(1000)
    , mTimeCount(0.0f)
    , mParticleSystem(nullptr)
{
}

float EmitterComponent::Random(float min, float max)
{
    return min + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (max - min)));
}





