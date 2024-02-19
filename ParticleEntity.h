#pragma once
#include "Entity.h"

class ParticleEntity :public Entity
{
public:
	ParticleEntity(class Game* game,
		const Vector3& Position = Vector3::Zero,
		const Vector3& Velocity = Vector3::Zero,
		float LifeTime = 0);
	~ParticleEntity();

public:

	Vector3 mVelocity;
	float mLifeTime;
};

