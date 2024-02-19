#include "MoveComponent.h"
#include "Entity.h"

MoveComponent::MoveComponent(class Entity* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
	, mStrafeSpeed(0.0f)
	, mUpwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;

		//回転を追加させるクォータニオンを作成
		Quaternion inc(Vector3::UnitZ, angle);
		//もとのrotと増分のクォータニオンを結合
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}

	if (!Math::NearZero(mUpwardSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetUp() * mUpwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}

}
