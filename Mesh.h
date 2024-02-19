#pragma once
#include <vector>
#include <string>
#include "Collision.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	//���b�V���̃��[�h/�A�����[�h
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//���̃��b�V���Ɋ��蓖�Ă�ꂽ���_�z����擾
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//�C���f�b�N�X����e�N�X�`�����擾
	class Texture* GetTexture(size_t index);
	//�V�F�[�_�[�����擾
	const std::string& GetShaderName() const { return mShaderName; }
	//�I�u�W�F�N�g��Ԃł̋��E���̔��a���擾
	float GetRadius() const { return mRadius; }
	const AABB& GetBox() const { return mBox; }
	float GetSpecPower() const { return mSpecPower; }
private:
	AABB mBox;
	//���b�V���̃e�N�X�`���Q
	std::vector<class Texture*> mTextures;
	//���b�V���̒��_�z��
	class VertexArray* mVertexArray;
	//�V�F�[�_�̖��O
	std::string mShaderName;
	//�I�u�W�F�N�g��Ԃł̋��E���̔��a���L�^
	float mRadius;
	float mSpecPower;
};