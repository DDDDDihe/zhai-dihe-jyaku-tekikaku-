#include "Math.h"
#include "AutoMoveComponent.h"
#include "Entity.h"
#include "Game.h"
#include <cstdlib>
#include <SDL/SDL.h>

AutoMoveComponent::AutoMoveComponent(class Entity* owner, float moveSpeed, float range)
    :Component(owner)
    , mRange(range)
    , mRandomGenerator(std::random_device{}())
    , mDistribution(0.0f, 1.0f)
{
    ResetDest();
}

void AutoMoveComponent::Update(float deltaTime)
{
    mTimeCount += deltaTime * 10;
    if (mTimeCount >= mMoveTime)
    {
        ResetDest();
    }
    else
    {
        //エンティテの位置を更新
        Vector3 pos = mOwner->GetPosition();
        //Vector3 delta = mDestination;
        Vector3 delta = mDestination - pos;
        float t = (mMoveTime - mTimeCount);
        Vector3 movement = Vector3(delta.x / t, delta.y / t, delta.z / t);

        pos += movement;

        mOwner->SetPosition(pos);
    }
}

void AutoMoveComponent::ResetDest()
{
    //mMoveTime = 40.0f;
    mTimeCount = 0;
    mDestination = Vector3(
        (mDistribution(mRandomGenerator) - 0.5f)* mRange,
        (mDistribution(mRandomGenerator) - 0.5f) * mRange,
        std::max(0.0f, (mDistribution(mRandomGenerator) - 0.5f) * mRange)
    );
    
}

