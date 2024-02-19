#include "ParticleSystem.h"
#include "BallEntity.h"
#include "Game.h"

ParticleSystem::ParticleSystem(class Entity* owner, int updateOrder)
    :Component(owner, updateOrder)
    , mEmitter(nullptr)
    , mOffsetRange(-3000.0f, 3000.0f)
{

}


void ParticleSystem::Update(float deltaTime)
{
    //update emitter
    if (mEmitter)
    {
        //count emit time to emit particle
        if (mEmitter->mTimeCount < mEmitter->mDuration)
        {
            mEmitter->mTimeCount += deltaTime * 60;
        }
        else
        {
            //reset time count
            mEmitter->mTimeCount = 0;

            //emit ball particle
            for (int i = 0; i < mEmitter->mEmitNumOnce; i++)
            {
                //emit
                {
                    Vector3 offset(mEmitter->Random(mOffsetRange.x, mOffsetRange.y),
                        mEmitter->Random(mOffsetRange.x, mOffsetRange.y),
                        mEmitter->Random(mOffsetRange.x, mOffsetRange.y));

                    ParticleEntity* ball = new ParticleEntity(
                        GetOwner()->GetGame(),
                        mEmitter->GetOwner()->GetPosition() + offset,
                        Vector3(0.0f, 0.0f, -50.0f),
                        10.0f);
                    mParticles.emplace_back(ball);
                }
            }
        }
    }

    //update particle's attributes
    {
        static std::list<ParticleEntity*> trash;

        for (auto p : mParticles)
        {
            Vector3 pos = p->GetPosition();
            pos += p->mVelocity * deltaTime * 5;
            p->SetPosition(pos);
            p->mLifeTime -= deltaTime;

            //particle and floor collision
            if (pos.z < -100.0f) {
                p->SetState(Entity::State::EDead);
                trash.push_back(p);
                continue;
            }

            if (p->mLifeTime <= 0)
            {
                p->SetState(Entity::State::EDead);
                trash.push_back(p);
            }
        }

        for (auto p : trash)
        {
            mParticles.remove(p);
        }

        trash.clear();
    }

}


