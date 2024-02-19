#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Entity* owner, Shader* shader)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
	, mShader(shader)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		//���[���h���W�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		//
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//�A�N�e�B�u�e�N�X�`���̐ݒ�
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		//���b�V���̒��_�z����A�N�e�B�u�ɂ���
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//�`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}