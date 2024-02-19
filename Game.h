#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <SDL/SDL_types.h>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);

	class Renderer* GetRenderer() { return mRenderer; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }
	class HUD* GetHUD() { return mHUD; }

	//UI���Ǘ�����
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	//�w���UISreen���X�^�b�N�Ƀv�b�V������
	void PushUI(class UIScreen* screen);
	
	class PlayerEntity* GetPlayer() { return mPlayerEntity; }

	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	//�G���e�B�e�B����
	void AddPlane(class PlaneEntity* plane);
	void RemovePlane(class PlaneEntity* plane);
	std::vector<class PlaneEntity*>& GetPlanes() { return mPlanes; }

	void AddSkyEntity(class SkyEntity* sky);
	void RemoveSkyEntity(class SkyEntity* sky);
	std::vector<class SkyEntity*>& GetSkyEntity() { return mSky; }

private:
	void Input();
	void HandleKeyPress(int key);
	void UpdateGame();
	void Output();
	void Load();
	void UnLoad();

	//�S�ẴG���e�B�e�B���Q�[����
	std::vector<class Entity*> mEntitys;
	std::vector<class UIScreen*> mUIStack;
	std::unordered_map<std::string, class Font*> mFonts;

	std::unordered_map<std::string, std::string> mText;
	//�y���f�B���O���G���e�B�e�B
	std::vector<class Entity*> mPendingEntitys;

	class Renderer* mRenderer;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	//�t���[�����v�Z
	Uint32 mTicksCount;
	GameState mGameState;

	//���G���e�B�e�B���X�V���邩�ǂ���
	bool mUpdatingEntitys;

	//�Q�[��
	std::vector<class PlaneEntity*> mPlanes;
	std::vector<class SkyEntity*> mSky;
	class PlayerEntity* mPlayerEntity;
	class SpriteComponent* mCorsshair;
};

