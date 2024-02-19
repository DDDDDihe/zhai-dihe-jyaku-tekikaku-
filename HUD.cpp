#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "PlayerEntity.h"
#include <algorithm>
#include "TargetComponent.h"

HUD::HUD(Game* game)
	:UIScreen(game)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBar = r->GetTexture("Assets/HealthBar.png");
	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader)
{
	//�\����
	Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);

	//���[�_�[
	const Vector2 cRadarPos(390.0f, -275.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	//�P�_
	for (Vector2& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}
	DrawTexture(shader, mRadarArrow, cRadarPos);

	//// Health bar
	//DrawTexture(shader, mHealthBar, Vector2(-350.0f, -350.0f));
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime)
{
	//�ʏ�̃J�[�\���ɖ߂�
	mTargetEnemy = false;
	//�������쐬
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	//�����L���X�g
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		//�G���e�B�e�B��TargetComponent�������Ă��邩
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mEntity)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(float deltaTime)
{
	//1�O�̃t���[���̋P�_�ʒu���N���A
	mBlips.clear();

	//�v���C���[�̈ʒu�����[�_�[���W�ɕϊ�
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	//���l�Ƀv���C���[�̑O���x�N�g����ϊ�
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	//atan2���g���ă��[�_�[�̉�]�����߂�
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	//�񎟌��̉�]�s��𓾂�
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	//�P�_�̈ʒu���擾����
	for (auto tc : mTargetComps)
	{
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);

		//�v���C���[����^�[�Q�b�g�ւ̃x�N�g�����v�Z
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		//�͈͓��ɂ��邩
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			//playerToTarget����ʏ�̃��[�_�[�̒��S����̃I�t�Z�b�g�ɕϊ�����
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			//blipPos����]����
			blipPos = Vector2::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}
}
