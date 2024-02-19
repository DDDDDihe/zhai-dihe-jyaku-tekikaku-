#pragma once
#include "Component.h"
#include "SDL/SDL.h"
class SpriteComponent : public Component
{
public:
	//描画順が低いほど遠くに置かれる
	SpriteComponent(class Entity* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader); //
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }
protected:
	//描画するテクスチャ
	class Texture* mTexture;
	int mDrawOrder;
	//テクスチャの幅と高さ
	int mTexWidth;
	int mTexHeight;
	bool mVisible;
};
