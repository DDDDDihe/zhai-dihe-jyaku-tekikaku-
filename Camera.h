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
	//�s�b�`�̊p���x
	float mPitchSpeed;
	//�ő�̃s�b�`�p�x
	float mMaxPitch;
	//���݂̃s�b�`
	float mPitch;
};
