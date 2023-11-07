/*
 *	@File	ResultUI.h
 *	@Brief	���U���g��UI�\���B
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTUI
#define RESULTUI

#include "Libraries/SystemDatas/DrawSprite.h"

 // ���[�h
enum SELECTION{ NEXT, RETRY, SELECT, LENGTH };

class SystemManager;
class ResultUI
{
private:
	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �Z���N�g
	SELECTION m_selectingScene;

	// �R�C���̖���
	int m_coinNum;

	// ���v�R�C����
	int m_oneCoins;
	int m_tenCoins;

	// �N���A�^�C��
	int m_oneTime;
	int m_tenTime;

	// �`��Ŏg�����
	struct TexInfo { DirectX::SimpleMath::Vector2 pos; float alpha; float scale; };
	TexInfo m_texInfo[LENGTH];
	RECT m_rect[LENGTH];

private:
	// �摜�g�嗦
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// �摜�̒��S�ʒu
	const float	FONT_HEIGHT = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// �摜�̂P�X�v���C�g�T�C�Y
	const int NUM_SIZE = 100;

	// �t�H���g�T�C�Y/�����x
	const float DEFAULT_FONT_SCALE = 1.85f;
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_SCALE = 2.1f;
	const float SELECT_FONT_ALPHA = 1.0f;

	// �t�H���g�̑I��ύX���̓���
	const float SELECT_CHANGE_FADE = 0.45f;
	const float NO_SELECT_TIME = 0.2f;

	// �t���X�N�T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:
	ResultUI();
	~ResultUI();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="system">�V�X�e���}�l�[�W��</param>
	/// <param name="windowSize">�E�B���h�E�T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="clearTime">�N���A�^�C��</param>
	/// <returns>�Ȃ�</returns>
	void Update(const int& clearTime);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name = "drawSelect">�Z���N�g��`�悷��t���O</param>
	/// <returns>�Ȃ�</returns>
	void Render(const bool& drawSelect);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

private:

	/// <summary>
	/// �����̕`��
	/// </summary>
	/// <param name="texScale">�e�N�X�`���̃X�P�[��</param>
	/// <param name="windowRate">��ʂ̊g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawNumber(DirectX::SimpleMath::Vector2 texScale, const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// ���C�A�E�g�̕`��
	/// </summary>
	/// <param name="windowRate">��ʂ̊g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawLayout(const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// �I�𕶎��̕`��
	/// </summary>
	/// <param name="windowRate">��ʂ̊g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawFonts(const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// �e�N�X�`���̏�Ԃ�ύX
	/// </summary>
	/// <param name="which">�I������</param>
	/// <returns>�Ȃ�</returns>
	void UpdateTexture(SELECTION which);

public:
	/// ���v�R�C�����̐ݒ�
	void SetCoins(const int& coinNum);
	// �I�𒆂̍��ڂ̐ݒ�
	void SetSelecting(const int& select) { m_selectingScene = SELECTION(select); }
};

#endif // RESULTUI