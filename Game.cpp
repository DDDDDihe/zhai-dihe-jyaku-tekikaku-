#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "PhysWorld.h"
#include "Entity.h"
#include "UIScreen.h"
#include "HUD.h"
#include "MeshComponent.h"
#include "PlayerEntity.h"
#include "PlaneEntity.h"
#include "SkyEntity.h"
#include "TargetEntity.h"
#include "BallEntity.h"
#include "ParticleSystem.h"
#include "PauseMenu.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

Game::Game()
	:mRenderer(nullptr)
	,mPhysWorld(nullptr)
	,mGameState(EGameplay)
	,mUpdatingEntitys(false)
{

}

//������
bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//renderer���쐬����
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	//physics world���쐬����
	mPhysWorld = new PhysWorld(this);

	//�����̃����_�����O������������
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	Load();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mGameState != EQuit)
	{
		Input();
		UpdateGame();
		Output();
	}
}

void Game::AddPlane(PlaneEntity* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneEntity* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

void Game::AddSkyEntity(SkyEntity* sky)
{
	mSky.emplace_back(sky);
}

void Game::RemoveSkyEntity(SkyEntity* sky)
{
	auto iter = std::find(mSky.begin(), mSky.end(), sky);
	mSky.erase(iter);
}

void Game::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mGameState = EQuit;
			break;

			// �L�[���ŏ��ɉ����ꂽ�Ƃ�
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				if (mGameState == EGameplay)
				{
					HandleKeyPress(event.key.keysym.sym);
				}
				else if (!mUIStack.empty())
				{
					mUIStack.back()->
						HandleKeyPress(event.key.keysym.sym);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (mGameState == EGameplay)
			{
				HandleKeyPress(event.button.button);
			}
			else if (!mUIStack.empty())
			{
				mUIStack.back()->
					HandleKeyPress(event.button.button);
			}
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (mGameState == EGameplay)
	{
		for (auto actor : mEntitys)
		{
			if (actor->GetState() == Entity::EActive)
			{
				actor->Input(state);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->Input(state);
	}

}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		//pause menu���쐬����
		new PauseMenu(this);
		break;

	case '1':
	{
		// Load English text
		LoadText("Assets/Japanese.gptext");
		break;
	}

	case '2':
	{
		// Load Chinese text
		LoadText("Assets/Chinese.gptext");
		break;
	}

	case '3':
	{
		// Load Japanese text
		LoadText("Assets/English.gptext");
		break;
	}

	case SDL_BUTTON_LEFT:
	{
		mPlayerEntity->Shoot();
		break;
	}

	default:
		break;
	}
}

void Game::UpdateGame()
{
	//deltaTime���v�Z����
	//�Ō�̃t���[������16ms���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	if (mGameState == EGameplay)
	{
		//�S�ẴG���e�B�e�B���X�V����
		mUpdatingEntitys = true; 
		for (auto entity : mEntitys)
		{
			entity->Update(deltaTime);
		}
		mUpdatingEntitys = false;

		//�y���^�b�N�X���̃G���e�B�e�B��mEntity�Ɉړ�����
		for (auto pending : mPendingEntitys)
		{
			pending->ComputeWorldTransform();
			mEntitys.emplace_back(pending);
		}
		mPendingEntitys.clear();

		//dead��Ԃ̃G���e�B�e�B��Վ��x�N�^�[�ɒǉ�����
		std::vector<Entity*> deadEntitys;
		for (auto entity : mEntitys)
		{
			if (entity->GetState() == Entity::EDead)
			{
				deadEntitys.emplace_back(entity);
			}
		}

		//dead ��ԃG���e�B�e�B���폜����
		for (auto entity : deadEntitys)
		{
			delete entity;
		}
	}

	//  UIscreens���X�V����
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete* iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::Output()
{
	mRenderer->Draw();
}

void Game::Load()
{
	// Load English text
	LoadText("Assets/Japanese.gptext");

	Entity* a = nullptr;
	TargetEntity* b = nullptr;
	Quaternion q;
	//MeshComponent* mc = nullptr;

	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			a = new PlaneEntity(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	//sky��ݒ�
	a = new SkyEntity(this);

   //���C�g��ݒ�
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	mHUD = new HUD(this);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);

	mPlayerEntity = new PlayerEntity(this);

	b = new TargetEntity(this);
	b->SetAutoMove(20.0f, 1500.0f , 30.0f)->SetPosition(Vector3(2000.0f, 500.0f, 100.0f));
	b = new TargetEntity(this);
	b->SetAutoMove(10.0f, 2500.0f , 25.0f)->SetPosition(Vector3(2500.0f, 1000.0f, 400.0f));
	b = new TargetEntity(this);
	b->SetAutoMove(25.0f, 1100.0f)->SetPosition(Vector3(2200.0f, -500.0f, 200.0f));
	b = new TargetEntity(this);
	b->SetPosition(Vector3(2450.0f, 0.0f, 200.0f));
	b = new TargetEntity(this);
	b->SetAutoMove(25.0f, 1700.0f)->SetPosition(Vector3(1900.0f, -1450.0f, 200.0f));
	b = new TargetEntity(this);
	b->SetAutoMove(30.0f, 1500.0f , 50.0f)->SetPosition(Vector3(2410.0f, 800.0f, 200.0f));

	a = new ParticleEntity(this);
	a->SetPosition(Vector3(0.0f, 0.0, 50.0));
	a->SetScale(0);
	ParticleSystem* ps = new ParticleSystem(a);
	ps->SetEmitter(new EmitterComponent(a));
}

void Game::UnLoad()
{
	//�G���e�B�e�B���폜����
	while (!mEntitys.empty())
	{
		delete mEntitys.back();
	}

	// Clear the UI stack
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
}


void Game::Shutdown()
{
	UnLoad();
	TTF_Quit();
	delete mPhysWorld;

	if (mRenderer)
	{
		mRenderer->Shutdown();
	}

	SDL_Quit();
}

void Game::AddEntity(Entity* entity)
{
	//�G���e�B�e�B���X�V���邽�߂ɁA�y���^�b�N�X��������K�v
	if (mUpdatingEntitys)
	{
		mPendingEntitys.emplace_back(entity);
	}

	else
	{
		mEntitys.emplace_back(entity);
	}
}

void Game::RemoveEntity(Entity* entity)
{
	//�G���e�B�e�B�̓y���^�b�N�X���ł��傤
	auto iter = std::find(mPendingEntitys.begin(), mPendingEntitys.end(), entity);
	if (iter != mPendingEntitys.end())
	{
		std::iter_swap(iter, mPendingEntitys.end() - 1);
		mPendingEntitys.pop_back();
	}

	iter = std::find(mEntitys.begin(), mEntitys.end(), entity);
	if (iter != mEntitys.end())
	{
		std::iter_swap(iter, mEntitys.end() - 1);
		mEntitys.pop_back();
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;

	}
}

void Game::LoadText(const std::string& fileName)
{
	//���łɃ��[�h����Ă���ꍇ�́A�����̃}�b�v���N���A����
	mText.clear();
	//�t�@�C�����J����
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}
	// �t�@�C���S�̂𕶎���X�g���[���ɓǂݍ���
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// ���̃t�@�C����rapidJSON�ŊJ��
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}
	// �e�L�X�g�}�b�v����͂���
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	//���̃e�L�X�g�����݂���ꍇ�A�}�b�v����T��
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}




