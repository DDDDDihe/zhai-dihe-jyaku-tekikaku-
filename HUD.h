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
	//十字線のためのテクスチャ
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;

	//全てのターゲットコンポーネントはこのゲームにある
	std::vector<class TargetComponent*> mTargetComps;
	//レーダー中心から輝点への2D相対オフセット
	std::vector<Vector2> mBlips;
	//レーダーの範囲と半径
	float mRadarRange;
	float mRadarRadius;
	//十字線が敵を捉えているか
	bool mTargetEnemy;
};
