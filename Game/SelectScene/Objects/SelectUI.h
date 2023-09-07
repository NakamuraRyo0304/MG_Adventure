/*
 *	@File	SelectUI.h
 *	@Brief	�Z���N�g��UI�\���B
 *	@Date	2023-09-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTUI
#define SELECTUI

class SystemManager;
class SelectUI
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

	// �L�[����
	float m_rightAlpha;
	float m_leftAlpha;

public:
	SelectUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);
	~SelectUI();

	// ����������
	void Initialize(const DirectX::SimpleMath::Vector2& windowSize);

	// �X�V����
	void Update(const float& timer, const bool& rightFlag, const bool& leftFlag);

	// �`�揈��
	void Render(const int& selectNum);

	// �I������
	void Finalize();
};

#endif // SELECTUI