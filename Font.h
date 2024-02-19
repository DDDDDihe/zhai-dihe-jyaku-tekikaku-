#pragma once
#include <string>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Math.h"

class Font
{
public:
	Font(class Game* game);
	~Font();

	//�t�@�C���̃��[�h/�A�����[�h
	bool Load(const std::string& filename);
	void Unload();

	//��������e�N�X�`���ɕ`��
	class Texture* RenderText(const std::string& textKey, const Vector3& color = Color::Blue, int pointSize = 30);

private:
	//�|�C���g�T�C�Y�ƃt�H���g�f�[�^�̘A�z�z��
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};

