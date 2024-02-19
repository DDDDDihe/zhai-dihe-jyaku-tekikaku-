#include "PlayerEntity.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"
#include "Camera.h"
#include "MeshComponent.h"
#include "BallEntity.h"
#include "BoxComponent.h"
#include "PlaneEntity.h"

PlayerEntity::PlayerEntity(Game* game)
	:Entity(game)
	, mPlayerState(PlayerState::InPlane)
	, mJumpSpeed(0.0f)
	, mGravity(-600.0f)
{
	mMoveComp = new MoveComponent(this);

	mCameraComp = new Camera(this);

	mPlayermodel = new Entity(game);
	mPlayermodel->SetScale(0.75f);
	mMeshComp = new MeshComponent(mPlayermodel, game->GetRenderer()->GetMeshShader_Phong());
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/miku_01.obj"));

	//ボックスコンポーネントを追加
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void PlayerEntity::UpdateEntity(float deltaTime)
{
	const uint8_t* keys = SDL_GetKeyboardState(nullptr);

	Entity::UpdateEntity(deltaTime);

	FixCollisions();

	//プレーヤーモデルは、エンティティとの相対位置により更新
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mPlayermodel->SetPosition(modelPos);
	//エンティティの回転で初期化
	Quaternion q = GetRotation();
	//カメラのピッチによる回転
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));
	mPlayermodel->SetRotation(q);

	Vector3 pos;
	switch (mPlayerState)
	{
	case PlayerState::InPlane:
		if (keys[SDL_SCANCODE_SPACE])
		{
			SetState(PlayerState::Jumping);
			mJumpSpeed = 400.0f;
		}
		break;

	case PlayerState::Jumping:
		mJumpSpeed += mGravity * deltaTime;
		pos = GetPosition() + GetUp() * mJumpSpeed * deltaTime;
		SetPosition(pos);

		if (mJumpSpeed <= 0.0f) {
			SetState(PlayerState::Falling);
		}
		break;

	case PlayerState::Falling:
		mJumpSpeed += deltaTime * mGravity;
		pos = GetPosition() + GetUp() * mJumpSpeed * deltaTime;
		SetPosition(pos);

		if (GetPosition().z < 0.0f) {
			SetPosition(Vector3(GetPosition().x, GetPosition().y, 0.0f));
			mJumpSpeed = 0.0f;
			SetState(PlayerState::InPlane);
		}
		break;
	default:
		break;
	}
}

void PlayerEntity::EntityInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}


	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// Mouse
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	const int maxMouseSpeed = 500;
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void PlayerEntity::Shoot()
{
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	BallEntity* ball = new BallEntity(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	ball->RotateToNewForward(dir);

}

void PlayerEntity::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void PlayerEntity::FixCollisions()
{
	//ワールド空間のボックスを更新するために
	//自分のワールド変換を再計算する必要がある
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		//PlaneEntityと衝突するか？
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			//全ての差を計算する
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			//dxには、dx1/dx2のうち絶対値が小さいほうがセットする
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			//dyも同様
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			//dzも同様
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			//x/y/zのうち最も差が小さい軸で位置を調整
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			//位置を設定しボックスの成分を更新する必要がある
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}

void PlayerEntity::SetState(PlayerState state)
{
	mPlayerState = state;
}
