/*
 *	@File	TitleUI.cpp
 *	@Brief	Start��Exit�̕\���B
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e���}�l�[�W��
#include "Libraries/SystemManager/SystemManager.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

#include "TitleUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="windowSize">��ʃT�C�Y</param>
 /// <returns>�Ȃ�</returns>
TitleUI::TitleUI(const SimpleMath::Vector2& windowSize)
	: is_selectFlag{}			// �I���t���O
	, m_windowSize{windowSize}	// �E�B���h�E�T�C�Y
	, m_sFontRate{}				// �X�^�[�g�t�H���g�̊g�嗦
	, m_eFontRate{}				// �G���h�t�H���g�̊g�嗦
	, m_sLineRate{}				// �X�^�[�g���C���̊g�嗦
	, m_eLineRate{}				// �G���h���C���̊g�嗦
	, m_startColor{}			// ���߂̐F
	, m_exitColor{}				// �I���̐F
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
TitleUI::~TitleUI()
{
	Finalize();
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
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

	// �g�嗦�̏�����
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ 0.0f,1.0f };
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="selectFlag">�J�nor�I���t���O</param>
/// <returns>�Ȃ�</returns>
void TitleUI::Update(const bool& selectFlag)
{
	if (selectFlag) // True�̎���Start
	{
		// �����̊g�嗦
		m_sFontRate = SimpleMath::Vector2{ SELECT_FONT_RATE };
		m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };

		// �����̊g�嗦
		m_sLineRate  = UserUtility::Lerp(m_sLineRate,  SimpleMath::Vector2::One,	     SimpleMath::Vector2{ 0.2f });
		m_eLineRate  = UserUtility::Lerp(m_eLineRate,  SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });

		// �����̓����x
		m_startColor = SimpleMath::Vector4::One;
		m_exitColor  = SimpleMath::Vector4{ 1.0f,1.0f,1.0f,0.5f };
	}
	else			// False�̎���Exit
	{
		// �����̊g�嗦
		m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
		m_eFontRate = SimpleMath::Vector2{ SELECT_FONT_RATE };

		// �����̊g�嗦
		m_sLineRate  = UserUtility::Lerp(m_sLineRate,  SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });
		m_eLineRate  = UserUtility::Lerp(m_eLineRate,  SimpleMath::Vector2::One,		 SimpleMath::Vector2{ 0.2f });

		// �����̓����x
		m_startColor = SimpleMath::Vector4{ 1.0f,1.0f,1.0f,0.5f };
		m_exitColor  = SimpleMath::Vector4::One;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleUI::Render()
{
	// ��ʔ䗦
	SimpleMath::Vector2 posRate = m_windowSize / FULL_SCREEN_SIZE * 0.5f;

	// �X�^�[�g�̕���--------------------------------(��ʔ䗦�̕ύX���l���������W�ɐݒ�)
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		m_startColor,
		posRate * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// �C�O�W�b�g�̕���------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		m_exitColor,
		posRate * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// �A���_�[���C��--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_sLineRate * posRate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_eLineRate * posRate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleUI::Finalize()
{
	m_system.reset();
}
