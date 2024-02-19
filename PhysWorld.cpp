#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL/SDL.h>

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	//closestT�́A������ŏ���������
	//�ŏ��̌����ŕK���X�V�����
	float closestT = Math::Infinity;
	Vector3 norm;
	//�S�Ẵ{�b�N�X���e�X�g����
	for (auto box : mBoxes)
	{
		float t;
		//�����̓{�b�N�X�ƌ������邩
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			//�O�̌��������߂���
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mEntity = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Entity*, Entity*)> f)
{
	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		//�������g�ƁA���łɃe�X�g����i�̒l�Ƃ́A�e�X�g���Ȃ�
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				//�񋟂��ꂽ�֐����Ăяo���Č����𔻒肳����
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Entity*, Entity*)> f)
{
	//min.x�ɂ���ă\�[�g����
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.x <
				b->GetWorldBox().mMin.x;
		});

	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		//box[i]��max.x���擾
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];
			//����AABB[j]��min.x���Abox[i]��max.x���E���z���Ă�����
			//box[i]�ƌ�������\��������{�b�N�X�́A���ɑ��݂��Ȃ�
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
	if (iter != mBoxes.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}
