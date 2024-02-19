
#include "TargetEntity.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "TargetComponent.h"


TargetEntity::TargetEntity(Game* game)
	:Entity(game)
{
	//SetScale(10.0f);
	SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	MeshComponent* mc = new MeshComponent(this, game->GetRenderer()->GetMeshShader_Phong());
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);
	//コリジョンボックスを追加
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
	new TargetComponent(this);

	//mAutoMove = new AutoMoveComponent(this, 20.0f, 1500.0f);
}


void TargetEntity::DeleteTarget()
{
	this->SetState(EDead);
}

TargetEntity* TargetEntity::SetAutoMove(float moveSpeed, float range , float moveTime)
{
	if (mAutoMove != nullptr)
	{
		delete mAutoMove;
		mAutoMove = nullptr;
	}

	mAutoMove = new AutoMoveComponent(this, moveSpeed, range);
	mAutoMove->SetMoveTime(moveTime);

	return this;
}
