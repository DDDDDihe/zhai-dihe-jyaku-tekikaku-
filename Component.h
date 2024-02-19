#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Entity* owner, int updateOrder = 100);
	
	virtual ~Component();

	virtual void Update(float deltaTime);

	virtual void Input(const uint8_t* keyState) {}

	virtual void OnUpdateWorldTransform() { }

	class Entity* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Entity* mOwner;
	int mUpdateOrder;

};

