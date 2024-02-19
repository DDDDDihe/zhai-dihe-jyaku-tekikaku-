#pragma once
#include <vector>
#include <string>
#include "Collision.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	//メッシュのロード/アンロード
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//このメッシュに割り当てられた頂点配列を取得
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//インデックスからテクスチャを取得
	class Texture* GetTexture(size_t index);
	//シェーダー名を取得
	const std::string& GetShaderName() const { return mShaderName; }
	//オブジェクト空間での境界球の半径を取得
	float GetRadius() const { return mRadius; }
	const AABB& GetBox() const { return mBox; }
	float GetSpecPower() const { return mSpecPower; }
private:
	AABB mBox;
	//メッシュのテクスチャ群
	std::vector<class Texture*> mTextures;
	//メッシュの頂点配列
	class VertexArray* mVertexArray;
	//シェーダの名前
	std::string mShaderName;
	//オブジェクト空間での境界球の半径を記録
	float mRadius;
	float mSpecPower;
};