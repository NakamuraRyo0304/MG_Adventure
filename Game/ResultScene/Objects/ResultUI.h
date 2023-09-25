/*
 *	@File	ResultUI.h
 *	@Brief	���U���g��UI�\���B
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTUI
#define RESULTUI

 // ���[�h
enum { RETRY, SELECT, TITLE };

class SystemManager;
class ResultUI
{
private:
	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �^�C�}�[
	float m_timer;

	// �Z���N�g
	int m_selectingScene;

	// �R�C���̖���
	int m_coinNum;

	// ���v�R�C����
	int m_oneCoins;
	int m_tenCoins;

	// �N���A�^�C��
	int m_oneTime;
	int m_tenTime;

	// �����x
	float m_retryAlpha;
	float m_selectAlpha;
	float m_titleAlpha;

	// �g�嗦
	float m_retryScale;
	float m_selectScale;
	float m_titleScale;

private:
	// �摜�g�嗦
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// �摜�̂P�X�v���C�g�T�C�Y
	const float NUM_SIZE = 100.0f;

	// �t�H���g�T�C�Y
	const float DEFAULT_FONT_SCALE = 2.0f;
	const float SELECT_FONT_SCALE = 2.1f;

	// �t�H���g�����x
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_ALPHA = 1.0f;

	// �t�H���g�̑I��ύX���̓���
	const float SELECT_CHANGE_FADE = 0.45f;


private:
	// �摜���W
	DirectX::SimpleMath::Vector2 m_retryPos;
	DirectX::SimpleMath::Vector2 m_selectPos;
	DirectX::SimpleMath::Vector2 m_titlePos;

public:
	ResultUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);
	~ResultUI();

	// ����������
	void Initialize(const DirectX::SimpleMath::Vector2& windowSize);

	// �X�V����
	void Update(const float& timer,const int& clearTime);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

private:
	void DrawNumber(DirectX::SimpleMath::Vector2 scale);

	// �e�I�����ڂ̕ύX
	void CaseRetry();
	void CaseSelect();
	void CaseTitle();

public:
	// �R�C�����Z�b�^�[
	void SetCoins(const int& coinNum);

	// �I�𒆂̍��ڂ̃Z�b�g
	void SetSelecting(const int& select) { m_selectingScene = select; }


};

#endif // RESULTUI