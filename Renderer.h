#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"


struct DirectionalLight
{
	//���̕���
	Vector3 mDirection;
	//�g�U���ːF
	Vector3 mDiffuseColor;
	//���ʔ��ːF
	Vector3 mSpecColor;

};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }

	Vector3 Unproject(const Vector3& screenPoint) const;

	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

	class Shader* GetMeshShader_Phong() { return mMeshShader_Phong; }
	class Shader* GetMeshShader_Basic() { return mMeshShader_Basic; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;

	std::vector<class SpriteComponent*> mSprites;
	//�S�Ẵ��b�V���̕`�揈���̃R���|�l���g
	std::vector<class MeshComponent*> mMeshComps; 

	// Game
	class Game* mGame;

	class Shader* mSpriteShader;
	class VertexArray* mSpriteVerts;

	class Shader* mMeshShader_Phong;
	class Shader* mMeshShader_Basic;

	Matrix4 mView;
	Matrix4 mProjection;
	float mScreenWidth;
	float mScreenHeight;

	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	
};

