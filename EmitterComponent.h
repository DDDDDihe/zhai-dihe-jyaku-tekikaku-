#pragma once
#include "Component.h"
#include "Math.h"

class ParticleSystem;

class EmitterComponent : public Component
{
public:
    EmitterComponent(class Entity* owner);

    float Random(float min, float max);

    //设置/获取每秒钟发射的粒子数
    void SetEmitRate(float emitRate) { mEmitNumOnce = emitRate; }
    float GetEmitRate() const { return mEmitNumOnce; }

    //设置获取系统中最大的粒子
    void SetMaxParticles(int maxParticles) { mMaxParticles = maxParticles; }
    int GetMaxParticles() const { return mMaxParticles; }

public:

    ParticleSystem* mParticleSystem;

    //每秒发射的粒子数
    float mEmitNumOnce;

    //系统中最大的粒子数
    int mMaxParticles;

    float mTimeCount; //保存上次发射结束后经过的时间 
    float mDuration;  //发射间隔时间

};
