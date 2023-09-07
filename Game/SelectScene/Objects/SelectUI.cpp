/*
 *	@File	SelectUI.cpp
 *	@Brief	�Z���N�g��UI�\���B
 *	@Date	2023-09-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e���}�l�[�W��
#include "Libraries/SystemManager/SystemManager.h"

#include "SelectUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="system">�V�X�e���}�l�[�W��</param>
 /// <param name="context">�R���e�L�X�g�|�C���^</param>
 /// <param name="device">�f�o�C�X�|�C���^</param>
 /// <returns>�Ȃ�</returns>
SelectUI::SelectUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
	:m_rightAlpha{}
	,m_leftAlpha{}
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜�̓o�^
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow", L"Resources/Textures/SELECT_INFO/RightArrow.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",  L"Resources/Textures/SELECT_INFO/LeftArrow.dds",  device);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SelectUI::~SelectUI()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="windowSize">��ʃT�C�Y</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Initialize(const SimpleMath::Vector2& windowSize)
{
	m_windowSize = windowSize;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">����</param>
/// <param name="rightFlag">�E�L�[������������</param>
/// <param name="leftFlag">���L�[������������</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Update(const float& timer, const bool& rightFlag, const bool& leftFlag)
{
	m_timer = timer;

	// �t���O�ɂ���ē����x��ς���
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha = !leftFlag ? 0.5f : 1.0f;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Render(const int& selectNum)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	if (selectNum != 9)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"RightArrow",
			SimpleMath::Vector2{ 1650.0f,360.0f} * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_rightAlpha },
			scale,
			SimpleMath::Vector2::Zero
		);
	}
	if (selectNum != 0)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"LeftArrow",
			SimpleMath::Vector2{ 20.0f ,360.0f } * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_leftAlpha },
			scale,
			SimpleMath::Vector2::Zero
		);
	}
}
/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectUI::Finalize()
{
	m_system.reset();
}
