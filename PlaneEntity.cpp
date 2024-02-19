#include "PlaneEntity.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneEntity::PlaneEntity(Game* game)
	:Entity(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this, game->GetRenderer()->GetMeshShader_Phong());
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);
	//コリジョンボックスを追加
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddPlane(this);
}

PlaneEntity::~PlaneEntity()
{
	GetGame()->RemovePlane(this);
}
