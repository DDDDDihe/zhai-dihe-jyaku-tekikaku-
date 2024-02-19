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

	//ファイルのロード/アンロード
	bool Load(const std::string& filename);
	void Unload();

	//文字列をテクスチャに描画
	class Texture* RenderText(const std::string& textKey, const Vector3& color = Color::Blue, int pointSize = 30);

private:
	//ポイントサイズとフォントデータの連想配列
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};

