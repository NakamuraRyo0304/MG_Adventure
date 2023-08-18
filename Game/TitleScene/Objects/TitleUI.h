/*
 *	@File	TitleUI.h
 *	@Brief	Start��Exit�̕\���B
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLEUI
#define TITLEUI

class SystemManager;
class TitleUI
{
private:
	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �Z���N�g�t���O
	bool is_selectFlag;

private:
	// �t���X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:

	TitleUI(const DirectX::SimpleMath::Vector2& windowSize);
	~TitleUI();

	// �쐬����
	void Create(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);

	// �X�V����
	void Update(const bool& selectFlag);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

};

#endif // TITLEUI