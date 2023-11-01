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
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
TitleUI::TitleUI()
	: is_selectFlag{}			// �I���t���O
	, m_windowSize{}			// �E�B���h�E�T�C�Y
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
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <param name="windowSize">�E�B���h�E�T�C�Y</param>
/// <param name="fullSize">�t���X�N���[���T�C�Y</param>
/// <returns>�Ȃ�</returns>
void TitleUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	// �V�X�e��
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// �摜�̒ǉ�
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

	// �g�嗦�̏�����
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
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
/// <param name="fadeValue">�t�F�[�h�̒l</param>
/// <param name="endAnim"�A�j���[�V�����̏�������</param>
/// <returns>�Ȃ�</returns>
void TitleUI::Render(const float& fadeValue, const bool& endAnim)
{
	// ��ʔ䗦
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize * 0.5f;

	// �t�F�[�h���͏������Ȃ�
	if (fadeValue >= 0.7f) return;

	// �A�j���[�V�������͏������Ȃ�
	if (!endAnim) return;

	// �X�^�[�g�̕���--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _rate,
		m_startColor,
		_rate * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// �C�O�W�b�g�̕���------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _rate,
		m_exitColor,
		_rate * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// �A���_�[���C��--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _rate,
		SimpleMath::Vector4::One,
		m_sLineRate * _rate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _rate,
		SimpleMath::Vector4::One,
		m_eLineRate * _rate,
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
