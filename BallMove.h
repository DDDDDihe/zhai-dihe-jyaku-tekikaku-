#pragma once
#include "MoveComponent.h"

class BallMove : public MoveComponent
{
public:
	BallMove(class Entity* owner);

	void SetPlayer(Entity* player) { mPlayer = player; }
	void Update(float deltaTime) override;
protected:
	class Entity* mPlayer;
};