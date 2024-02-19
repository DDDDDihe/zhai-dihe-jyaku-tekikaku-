#pragma once
#include <vector>
#include <functional>
#include "Math.h"
#include "Collision.h"

class PhysWorld
{
public:
	PhysWorld(class Game* game);

	struct CollisionInfo
	{
		//衝突した点
		Vector3 mPoint;
		//衝突した点法線
		Vector3 mNormal;
		//交差したコンポーネント
		class BoxComponent* mBox;
		//コンポーネントを所有するアクター
		class Entity* mEntity;
	};

	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	void TestPairwise(std::function<void(class Entity*, class Entity*)> f);

	void TestSweepAndPrune(std::function<void(class Entity*, class Entity*)> f);

	//ワールドからボックスコンポーネントを追加・削除する
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
};

