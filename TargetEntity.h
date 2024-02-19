
#pragma once
#include "Entity.h"
#include "AutoMoveComponent.h"

class TargetEntity : public Entity
{
public:
	TargetEntity(class Game* game);
	void DeleteTarget();
	TargetEntity* SetAutoMove(float moveSpeed, float range, float moveTime = 40.0f);

private:
	class AutoMoveComponent* mAutoMove = nullptr;
};
