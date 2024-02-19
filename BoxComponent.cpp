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
	//�I�u�W�F�N�g��Ԃ̃{�b�N�X�Ń��Z�b�g
	mWorldBox = mObjectBox;
	//�X�P�[�����O
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	//��]
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//���s�ړ�
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}
