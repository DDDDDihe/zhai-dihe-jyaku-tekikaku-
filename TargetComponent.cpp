#include "BoxComponent.h"
#include "Entity.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetComponent.h"
#include "Math.h"
#include "Game.h"
#include "HUD.h"

TargetComponent::TargetComponent(Entity* owner)
	:Component(owner)
{
	mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent()
{
	mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}
