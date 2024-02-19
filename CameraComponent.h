#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Entity* owner, int updateOrder = 200);
protected:
	void SetViewMatrix(const Matrix4& view);
};
