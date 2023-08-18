/*
 *	@File	TitleUI.cpp
 *	@Brief	Start��Exit�̕\���B
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e���}�l�[�W��
#include "Libraries/SystemManager/SystemManager.h"

#include "TitleUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
TitleUI::TitleUI(const SimpleMath::Vector2& windowSize):
	is_selectFlag{},
	m_windowSize{windowSize}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
TitleUI::~TitleUI()
{
}

/// <summary>
/// �쐬�֐�
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="context">�R���e�L�X�g�|�C���^</param>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void TitleUI::Create(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �V�X�e��
	m_system = system;

	// �摜�̐ݒ�
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜�̒ǉ�
	m_system->GetDrawSprite()->AddTextureData(L"Start", L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit", L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="selectFlag">�J�nor�I���t���O</param>
/// <returns>�Ȃ�</returns>
void TitleUI::Update(const bool& selectFlag)
{
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleUI::Render()
{
	SimpleMath::Vector2 posRate = m_windowSize / FULL_SCREEN_SIZE;

	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 100.0f,700.0f } * posRate,
		SimpleMath::Color{ 1.0f,1.0f,1.0f,1.0f },
		1.0f * posRate,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleUI::Finalize()
{
}
