#include "Camera.h"
#include "Entity.h"

Camera::Camera(Entity* owner)
	:CameraComponent(owner)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{
}

void Camera::Update(float deltaTime)
{
	//親クラスのUpdateを呼び出す
	CameraComponent::Update(deltaTime);
	//カメラの位置は所有エンティティの位置
	Vector3 cameraPos = mOwner->GetPosition();

	//ピッチの角速度に基づいてピッチを更新
	mPitch += mPitchSpeed * deltaTime;
	//ピッチを[-max, +max]の範囲に収める
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	//ピッチ回転を表すクォータニオンを作る
	//オーナーの右向きベクトルを軸とする回転
	Quaternion q(mOwner->GetRight(), mPitch);

	//ピッチのクォータニオンで、所有エンティティの前方ベクトルを回転
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	//ターゲットの位置は全てのエンティティの前方100単位
	Vector3 target = cameraPos + viewForward * 100.0f;

	//上方ベクトルもピッチのクォータニオンで回転
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	//注視行列を作って、ビューに設定する
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
