#pragma once
#include "CameraComponent.h"

class Camera : public CameraComponent
{
public:
	Camera(class Entity* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }
private:
	//ピッチの角速度
	float mPitchSpeed;
	//最大のピッチ角度
	float mMaxPitch;
	//現在のピッチ
	float mPitch;
};
