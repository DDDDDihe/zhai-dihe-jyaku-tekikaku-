#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Entity.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Entity* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		// 幅と高さを所有エンティティのスケールで拡縮
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		shader->SetMatrixUniform("uWorldTransform", world);
		mTexture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	//幅と高さを設定
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
