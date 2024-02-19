#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Button
{
public:
	//�|�^���̈ʒu�y�ѐ��@���󂯎��
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims);
	~Button();

	//�\���e�L�X�g��ݒ肵�A�e�N�X�`���𐶐�����
	void SetName(const std::string& name);

	class Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }

	//���W���{�^���͈͓̔��Ȃ�true��Ԃ�
	bool ContainsPoint(const Vector2& pt) const;
	//�{�^���������ꂽ���ɌĂяo�����
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
	// UIScreen�̔h���N���X�ȉ����I�[�o�[���C�h�ł���
	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);
	virtual void Input(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	//��Ԃ��A�N�e�B�u���N���[�W���O�����Ǘ�
	enum UIState
	{
		EActive,
		EClosing
	};
	//��Ԃ��N���[�W���O�ɂ���
	void Close();
	//��Ԃ��擾
	UIState GetState() const { return mState; }
	//�^�C�g���̕������ύX
	void SetTitle(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 40);

	void AddButton(const std::string& name, std::function<void()> onClick);
protected:
	//�e�N�X�`����`�悷��w���p�[�֐�
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);

	void SetRelativeMouseMode(bool relative);
	class Game* mGame;

	//UI��ʂ̃^�C�g��������p
	class Font* mFont;
	class Texture* mTitle;
	class Texture* mBackground;
	class Texture* mButtonOn;
	class Texture* mButtonOff;

	Vector2 mTitlePos;
	Vector2 mNextButtonPos;
	Vector2 mBGPos;

	//���
	UIState mState;

	std::vector<Button*> mButtons;
};