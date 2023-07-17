/*
 *	@File	PlayUI.h
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYUI
#define PLAYUI

class SystemManager;
class PlayUI
{
private:
	// �V�F�A�|�C���^
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �^�C�}�[�̍��W(1�̈�)
	DirectX::SimpleMath::Vector2 m_oneSecPos;
	// �^�C�}�[�̍��W(10�̈�)
	DirectX::SimpleMath::Vector2 m_tenSecPos;

	// ��������
	int m_timeLimit;

	// �����G�t�F�N�g�t���O
	bool is_effectFlag;


public:
	PlayUI(const DirectX::SimpleMath::Vector2& windowSize);
	~PlayUI();

	// �쐬����
	void Create(std::shared_ptr<SystemManager> system ,ID3D11DeviceContext1* context, ID3D11Device1* device);

	// �X�V����
	void Update(const float& timelimit);

	// �`�揈��
	void Render();

	// �f�B�W�b�g�̌v�Z
	void RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, float scale, int digitWidth, int digitHeight);

	// �I������
	void Finalize();

public:

	// �G�t�F�N�g�t���O�̃Z�b�g
	void SetEffectFlag(const bool& flag) { is_effectFlag = flag; }
};

#endif //PLAYUI
