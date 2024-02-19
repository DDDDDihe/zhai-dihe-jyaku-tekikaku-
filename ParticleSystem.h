#pragma once
//コンポーネント指向
#include "Component.h"
#include "Renderer.h"
#include "EmitterComponent.h"
#include "ParticleEntity.h"
#include <vector>
#include "Entity.h"
#include <list>


class ParticleSystem : public Component
{
public:
    ParticleSystem(class Entity* owner, int updateOrder = 100);

    void Update(float deltaTime) override;

    void SetEmitter(EmitterComponent* emitter) { mEmitter = emitter; }
    EmitterComponent* GetEmitter() { return mEmitter; }

private:
    std::list<ParticleEntity*> mParticles;

    EmitterComponent* mEmitter;
    Vector2 mOffsetRange;
};
