/*
 *	@File	PlayUI.cpp
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

#include "PlayUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
PlayUI::PlayUI(const DirectX::SimpleMath::Vector2& windowSize):
	m_systemManager{},
	m_windowSize{ windowSize }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayUI::~PlayUI()
{
	Finalize();
	m_systemManager.reset();
}

/// <summary>
/// �쐬����
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Create(std::shared_ptr<SystemManager> system ,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �V�X�e��
	m_systemManager = system;

	// �摜�̐ݒ�
	m_systemManager->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜��o�^
	m_systemManager->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);

	// �䗦���v�Z
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_timerPos = { m_windowSize.x / 2 * span , 80 * span };
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timelimit">�c�莞��</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Update(const float& timelimit)
{
	timelimit;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Render()
{
	// �䗦���v�Z
	float scale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	RECT_U rect;

	rect.left = 0;
	rect.right = rect.left + 1000;
	rect.bottom = 100;

	int num = (rect.left / 100) ;

	SimpleMath::Vector2 Center = {
	static_cast<float>(rect.right / 2),
	static_cast<float>(rect.bottom / 2)};


	m_systemManager->GetDrawSprite()->DrawTexture(
		L"Number",							// �o�^�L�[
		m_timerPos,							// ���W
		{ 1.0f,1.0f,1.0f,1.0f },			// �F
		1.0f * scale,						// �g�嗦
		Center,								// ���S�ʒu
		rect								// �؂���ʒu
	);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Finalize()
{
}
