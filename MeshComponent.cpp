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
		//ワールド座標変換の設定
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		//
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//アクティブテクスチャの設定
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		//メッシュの頂点配列をアクティブにする
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
