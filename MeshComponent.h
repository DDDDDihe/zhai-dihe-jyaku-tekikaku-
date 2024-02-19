#pragma once
#include "Component.h"
#include <cstddef>
#include "Mesh.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Entity* owner, class Shader* shader);
	~MeshComponent();
	//このメッシュコンポーネントを指定のシェーダーで描画する
	virtual void Draw(class Shader* shader);
	//メッシュコンポーネントが使うmesh/textureインデックスの設定
	virtual void SetMesh(Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }
protected:
	Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;

public:
	class Shader* mShader;
};
