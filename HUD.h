#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	class Texture* mHealthBar;
	class Texture* mRadar;
	//�\�����̂��߂̃e�N�X�`��
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;

	//�S�Ẵ^�[�Q�b�g�R���|�[�l���g�͂��̃Q�[���ɂ���
	std::vector<class TargetComponent*> mTargetComps;
	//���[�_�[���S����P�_�ւ�2D���΃I�t�Z�b�g
	std::vector<Vector2> mBlips;
	//���[�_�[�͈̔͂Ɣ��a
	float mRadarRange;
	float mRadarRadius;
	//�\�������G�𑨂��Ă��邩
	bool mTargetEnemy;
};
