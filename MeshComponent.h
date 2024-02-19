#pragma once
#include "Component.h"
#include <cstddef>
#include "Mesh.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Entity* owner, class Shader* shader);
	~MeshComponent();
	//���̃��b�V���R���|�[�l���g���w��̃V�F�[�_�[�ŕ`�悷��
	virtual void Draw(class Shader* shader);
	//���b�V���R���|�[�l���g���g��mesh/texture�C���f�b�N�X�̐ݒ�
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
