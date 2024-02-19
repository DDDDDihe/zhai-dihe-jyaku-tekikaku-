#include "Entity.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Entity::Entity(Game* game)
	:mState(EActive)
	,mPosition(Vector3::Zero)
	,mRotation(Quaternion::Identity)
	,mScale(1.0f)
	,mGame(game)
	,mRecomputeWorldTransform(true)
{
	mGame->AddEntity(this);
}

Entity::~Entity()
{
	mGame->RemoveEntity(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Entity::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);   //system
		UpdateEntity(deltaTime);

		ComputeWorldTransform();
	}
}

void Entity::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Entity::UpdateEntity(float deltaTime)
{

}

void Entity::Input(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->Input(keyState);
		}
		EntityInput(keyState);
	}
}

void Entity::EntityInput(const uint8_t* keyState)
{

}

void Entity::RotateToNewForward(const Vector3& forward)
{
	//元の方向と新しい方向の差を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	//方向はXか？
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	//方向は‐Xか？
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		//クロス積で得た軸周りに回転
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Entity::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Entity::AddComponent(Component* component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();

	for (; iter != mComponents.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, component);
}

void Entity::RemoveComponent(Component* component)

{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}