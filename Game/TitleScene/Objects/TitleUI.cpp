/*
 *	@File	TitleUI.cpp
 *	@Brief	Start��Exit�̕\���B
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "TitleUI.h"

// �R���X�g���N�^
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

// �f�X�g���N�^
TitleUI::~TitleUI()
{
	Finalize();
}

// �쐬�֐�
void TitleUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	// �V�X�e��
	m_system = system;
	m_windowSize = windowSize;

	// �摜�̒ǉ�
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds");
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds");
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds");

	// �g�嗦�̏�����
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
}

// �X�V����
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

// �`�揈��
void TitleUI::Render(const float& fadeValue, const bool& endAnim)
{
	// ��ʔ䗦
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE * 0.5f;

	// �t�F�[�h���͏������Ȃ�
	if (fadeValue >= 0.7f) return;

	// �A�j���[�V�������͏������Ȃ�
	if (!endAnim) return;

	// �X�^�[�g�̕���--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _scale,
		m_startColor,
		_scale * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// �C�O�W�b�g�̕���------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _scale,
		m_exitColor,
		_scale * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// �A���_�[���C��--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _scale,
		SimpleMath::Vector4::One,
		m_sLineRate * _scale,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _scale,
		SimpleMath::Vector4::One,
		m_eLineRate * _scale,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
}

// �I������
void TitleUI::Finalize()
{
	m_system.reset();
}
