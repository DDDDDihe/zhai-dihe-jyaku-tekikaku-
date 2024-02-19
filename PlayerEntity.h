#pragma once
#include "Entity.h"

class PlayerEntity : public Entity
{
public:
	enum class PlayerState
	{
		InPlane,
		Jumping,
		Falling
	};

	PlayerEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void EntityInput(const uint8_t* keys) override;

	void Shoot();
	
	void SetVisible(bool visible);

	void FixCollisions();

private:
	void SetState(PlayerState state);
	PlayerState mPlayerState;
	class MoveComponent* mMoveComp;
	class MeshComponent* mMeshComp;
	class Camera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Entity* mPlayermodel;

	float mGravity;
	float mJumpSpeed;
};

