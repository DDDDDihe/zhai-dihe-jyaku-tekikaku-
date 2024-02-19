#include "ParticleEntity.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"

ParticleEntity::ParticleEntity(Game* game,
	const Vector3& Position,
	const Vector3& Velocity,
	float LifeTime)
	:Entity(game)
{
	SetPosition(Position);
	mVelocity = Velocity;
	mLifeTime = LifeTime;

	MeshComponent* mc = new MeshComponent(this, game->GetRenderer()->GetMeshShader_Basic());
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);

}

ParticleEntity::~ParticleEntity()
{
}
