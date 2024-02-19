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
		//�Փ˂����_
		Vector3 mPoint;
		//�Փ˂����_�@��
		Vector3 mNormal;
		//���������R���|�[�l���g
		class BoxComponent* mBox;
		//�R���|�[�l���g�����L����A�N�^�[
		class Entity* mEntity;
	};

	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	void TestPairwise(std::function<void(class Entity*, class Entity*)> f);

	void TestSweepAndPrune(std::function<void(class Entity*, class Entity*)> f);

	//���[���h����{�b�N�X�R���|�[�l���g��ǉ��E�폜����
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
};

