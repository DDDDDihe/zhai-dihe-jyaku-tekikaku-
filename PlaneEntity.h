#pragma once
#include "Entity.h"

class PlaneEntity : public Entity
{
public:
	PlaneEntity(class Game* game);
	~PlaneEntity();
	class BoxComponent* GetBox() { return mBox; }
private:
	class BoxComponent* mBox;
};
