#include "CameraComponent.h"
#include "Entity.h"
#include "Renderer.h"
#include "Game.h"

CameraComponent::CameraComponent(Entity* owner, int updateOrder)
	:Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	//ビュー行列をレンダラーに渡す
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}
