#include "SkyEntity.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"

SkyEntity::SkyEntity(Game* game)
	:Entity(game)
{
	SetScale(550.0f);
	MeshComponent* mc = new MeshComponent(this, game->GetRenderer()->GetMeshShader_Basic());
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/sky.gpmesh");
	mc->SetMesh(mesh);

	game->AddSkyEntity(this);
}

SkyEntity::~SkyEntity()
{
	GetGame()->RemoveSkyEntity(this);
}
