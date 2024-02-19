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
	//進行方向の線分を構築
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment l(start, end);

	//線分とワールドの衝突を判定
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.mEntity != mPlayer)
	{
		//衝突したら法線の向きで方向を反射させる
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

	//前進速度を基準として基底クラスで動きを更新する
	MoveComponent::Update(deltaTime);
}
