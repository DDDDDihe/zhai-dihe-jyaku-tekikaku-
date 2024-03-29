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
    float mRange; //移動範囲
    float mMoveTime;
    float mTimeCount;
    Vector3 mDestination;
    std::mt19937 mRandomGenerator; //ランダム数字を生成
    std::uniform_real_distribution<float> mDistribution; // 均�鴛ｪ布随机数生成器

    void ResetDest();
};
