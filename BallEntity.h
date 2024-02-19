#pragma once
#include "Entity.h"

class BallEntity : public Entity
{
public:
	BallEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void SetPlayer(Entity* player);

	void HitTarget();
private:
	class BallMove* mMyMove;
	float mLifeSpan;
};
