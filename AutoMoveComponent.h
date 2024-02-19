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
    float mRange; //�ړ��͈�
    float mMoveTime;
    float mTimeCount;
    Vector3 mDestination;
    std::mt19937 mRandomGenerator; //�����_�������𐶐�
    std::uniform_real_distribution<float> mDistribution; // �������z������������

    void ResetDest();
};
