#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Button
{
public:
	//ポタンの位置及び寸法を受け取る
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims);
	~Button();

	//表示テキストを設定し、テクスチャを生成する
	void SetName(const std::string& name);

	class Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }

	//座標がボタンの範囲内ならtrueを返す
	bool ContainsPoint(const Vector2& pt) const;
	//ボタンが押された時に呼び出される
	void OnClick();
private:
	std::function<void()> mOnClick;
	std::string mName;
	class Texture* mNameTex;
	class Font* mFont;
	Vector2 mPosition;
	Vector2 mDimensions;
	bool mHighlighted;
};

class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// UIScreenの派生クラス以下をオーバーライドできる
	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);
	virtual void Input(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	//状態がアクティブかクロージングかを管理
	enum UIState
	{
		EActive,
		EClosing
	};
	//状態をクロージングにする
	void Close();
	//状態を取得
	UIState GetState() const { return mState; }
	//タイトルの文字列を変更
	void SetTitle(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 40);

	void AddButton(const std::string& name, std::function<void()> onClick);
protected:
	//テクスチャを描画するヘルパー関数
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);

	void SetRelativeMouseMode(bool relative);
	class Game* mGame;

	//UI画面のタイトル文字列用
	class Font* mFont;
	class Texture* mTitle;
	class Texture* mBackground;
	class Texture* mButtonOn;
	class Texture* mButtonOff;

	Vector2 mTitlePos;
	Vector2 mNextButtonPos;
	Vector2 mBGPos;

	//状態
	UIState mState;

	std::vector<Button*> mButtons;
};
