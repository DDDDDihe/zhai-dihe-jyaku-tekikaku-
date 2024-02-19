#pragma once
#include "MoveComponent.h"
#include <random>

class AutoMoveComponent : public Component
{
public:
    AutoMoveComponent(class Entity* owner, float moveSpeed, float range);
    void Update(float deltaTime) override;
    void SetMoveTime(float moveTime) { mMoveTime = moveTime; }

private:
    float mRange; //ˆÚ“®”ÍˆÍ
    float mMoveTime;
    float mTimeCount;
    Vector3 mDestination;
    std::mt19937 mRandomGenerator; //ƒ‰ƒ“ƒ_ƒ€”š‚ğ¶¬
    std::uniform_real_distribution<float> mDistribution; // ‹Ïú‰•ª•zŠ÷”¶¬Ší

    void ResetDest();
};
