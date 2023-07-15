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
	std::shared_ptr<SystemManager> m_systemManager;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �^�C�}�[�̍��W
	DirectX::SimpleMath::Vector2 m_timerPos;


public:
	PlayUI(const DirectX::SimpleMath::Vector2& windowSize);
	~PlayUI();

	// �쐬����
	void Create(std::shared_ptr<SystemManager> system ,ID3D11DeviceContext1* context, ID3D11Device1* device);

	// �X�V����
	void Update(const float& timelimit);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();
};

#endif //PLAYUI
