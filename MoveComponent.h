#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Entity* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }
	float GetUpwardSpeed() const { return mUpwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
	void SetUpwardSpeed(float speed) { mUpwardSpeed = speed; }

protected:
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
	float mUpwardSpeed;
};
