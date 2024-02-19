#include "BoxComponent.h"
#include "Entity.h"
#include "Game.h"
#include "PhysWorld.h"

BoxComponent::BoxComponent(Entity* owner, int updateOrder)
	:Component(owner,updateOrder)
	,mObjectBox(Vector3::Zero,Vector3::Zero)
	,mWorldBox(Vector3::Zero,Vector3::Zero)
	,mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform()
{
	//オブジェクト空間のボックスでリセット
	mWorldBox = mObjectBox;
	//スケーリング
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	//回転
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//平行移動
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}
