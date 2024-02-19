#include "BallEntity.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BallMove.h"

BallEntity::BallEntity(Game* game)
	:Entity(game)
	, mLifeSpan(2.0f)
{
	//SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this, game->GetRenderer()->GetMeshShader_Phong());
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);
	mMyMove = new BallMove(this);
	mMyMove->SetForwardSpeed(1500.0f);
}

void BallEntity::UpdateEntity(float deltaTime)
{
	Entity::UpdateEntity(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(EDead);
	}
}

void BallEntity::SetPlayer(Entity* player)
{
	mMyMove->SetPlayer(player);
}

void BallEntity::HitTarget()
{
	//
}
