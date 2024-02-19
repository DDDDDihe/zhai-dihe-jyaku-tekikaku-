#include "BallMove.h"
#include "Entity.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetEntity.h"
#include "BallEntity.h"

BallMove::BallMove(Entity* owner)
	:MoveComponent(owner)
{
}

void BallMove::Update(float deltaTime)
{
	//�i�s�����̐������\�z
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment l(start, end);

	//�����ƃ��[���h�̏Փ˂𔻒�
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.mEntity != mPlayer)
	{
		//�Փ˂�����@���̌����ŕ����𔽎˂�����
		dir = Vector3::Reflect(dir, info.mNormal);
		mOwner->RotateToNewForward(dir);
		TargetEntity* target = dynamic_cast<TargetEntity*>(info.mEntity);
		if (target)
		{
			auto ball = static_cast<BallEntity*>(mOwner);
			ball->HitTarget();
			ball->SetState(Entity::State::EDead);
			target->DeleteTarget();
		}
	}

	//�O�i���x����Ƃ��Ċ��N���X�œ������X�V����
	MoveComponent::Update(deltaTime);
}
