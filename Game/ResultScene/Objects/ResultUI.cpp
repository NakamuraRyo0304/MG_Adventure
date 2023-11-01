/*
 *	@File	ResultUI.cpp
 *	@Brief	���U���g��UI�\���B
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */


#include "pch.h"

 // �V�X�e���}�l�[�W��
#include "Libraries/SystemManager/SystemManager.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

#include "ResultUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
ResultUI::ResultUI()
	: m_selectingScene{ RETRY }	// ���ݑI�𒆂̃V�[��
	, m_oneCoins{}				//  1�̈ʂ̃R�C����
	, m_tenCoins{}				// 10�̈ʂ̃R�C����
	, m_oneTime{}				//  1�̈ʂ̃N���A�^�C��
	, m_tenTime{}				// 10�̈ʂ̃N���A�^�C��
	, m_retryInfo{}				// ���g���C�t�H���g�̏��
	, m_selectInfo{}			// �Z���N�g�t�H���g�̏��
	, m_titleInfo{}				// �^�C�g���t�H���g�̏��
	, m_coinNum{ 0 }			// ����
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ResultUI::~ResultUI()
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
void ResultUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds",		device);
	m_system->GetDrawSprite()->AddTextureData(L"RFont",  L"Resources/Textures/ResultFonts.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BLIND",  L"Resources/Textures/ResultBack.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"RETRY",  L"Resources/Textures/FONT/RETRY.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"TITLE",  L"Resources/Textures/FONT/TITLE.dds",	device);

	SimpleMath::Vector2 _scale = m_windowSize / m_fullScreenSize;

	// �����̍��W�̏�����
	float _commonY = m_fullScreenSize.y - NUM_SIZE * 2;
	m_retryInfo.pos  = SimpleMath::Vector2{ FONT_WIDTH / 2,					 _commonY };
	m_selectInfo.pos = SimpleMath::Vector2{ m_retryInfo.pos.x  + FONT_WIDTH, _commonY };
	m_titleInfo.pos  = SimpleMath::Vector2{ m_selectInfo.pos.x + FONT_WIDTH, _commonY };
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="clearTime">�N���A�^�C��</param>
/// <returns>�Ȃ�</returns>
void ResultUI::Update(const int& clearTime)
{
	// �N���A�^�C����ۑ�
	m_oneTime = clearTime % 10;
	m_tenTime = clearTime / 10 % 10;

	// �A���t�@�l�ƃX�P�[���̕ύX
	switch (m_selectingScene)
	{
	case RETRY:
		CaseRetry();
		break;
	case SELECT:
		CaseSelect();
		break;
	case TITLE:
		CaseTitle();
		break;
	default:
		break;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name = "fadeValue">�t�F�[�h�̒l</param>
/// <returns>�Ȃ�</returns>
void ResultUI::Render(const float& fadeValue)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize;

	// ���C�A�E�g�̕`��
	DrawBack(_rate);

	// ������`��
	DrawNumber(DRAW_NUM_SIZE, _rate);

	// �t�F�[�h���͏������Ȃ�
	if (fadeValue >= 0.7f) return;

	// �V�[���I�𕶎�(���g���C�A�Z���N�g�A�^�C�g��)
	DrawFonts(_rate);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::Finalize()
{
	m_system.reset();
}

/// <summary>
/// �����̕`��
/// </summary>
/// <param name="texScale">�e�N�X�`���̃X�P�[��</param>
/// <param name="windowRate">��ʂ̊g�嗦</param>
/// <returns>�Ȃ�</returns>
void ResultUI::DrawNumber(SimpleMath::Vector2 texScale, const SimpleMath::Vector2& windowRate)
{
	// �N���A����
	// �؂���ʒu�ݒ�
	RECT_U _oneRec = { m_oneTime * 100, 0,m_oneTime * 100 + 100, 100 };
	RECT_U _tenRec = { m_tenTime * 100, 0,m_tenTime * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,290.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,290.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	//-------------------------------------------------------------------------------------//
	// �R�C���̐�
	// �؂���ʒu�ݒ�
	_oneRec = { m_oneCoins * 100, 0,m_oneCoins * 100 + 100, 100 };
	_tenRec = { m_tenCoins * 100, 0,m_tenCoins * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,700.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,700.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_tenRec
	);
}

/// <summary>
/// ���C�A�E�g�̕`��
/// </summary>
/// <param name="windowRate">��ʂ̊g�嗦</param>
/// <returns>�Ȃ�</returns>
void ResultUI::DrawBack(const SimpleMath::Vector2& windowRate)
{
	// ��ʂ𔖈Â�����
	m_system->GetDrawSprite()->DrawTexture(
		L"BLIND",									// �o�^�L�[
		SimpleMath::Vector2::Zero,					// ���W
		{ 1.0f,1.0f,1.0f,0.3f },					// �F
		DEFAULT_RATE * windowRate,					// �g�嗦
		SimpleMath::Vector2::Zero					// ���S�ʒu
	);

	// ���̕���
	m_system->GetDrawSprite()->DrawTexture(
		L"RFont",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		DEFAULT_RATE * windowRate,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// �I�𕶎��̕`��
/// </summary>
/// <param name="windowRate">��ʂ̊g�嗦</param>
/// <returns>�Ȃ�</returns>
void ResultUI::DrawFonts(const DirectX::SimpleMath::Vector2& windowRate)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"RETRY",
		m_retryInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_retryInfo.alpha },
		IMAGE_RATE * windowRate * m_retryInfo.scale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"SELECT",
		m_selectInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_selectInfo.alpha },
		IMAGE_RATE * windowRate * m_selectInfo.scale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"TITLE",
		m_titleInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_titleInfo.alpha },
		IMAGE_RATE * windowRate * m_titleInfo.scale,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// ���v�R�C�����̌v�Z
/// </summary>
/// <param name="totalCoinNum">�R�C����</param>
/// <returns>�Ȃ�</returns>
void ResultUI::SetCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
}

/// <summary>
/// ���g���C��I�𒆂̃e�N�X�`���̏��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::CaseRetry()
{
	// �����x
	m_retryInfo.alpha = UserUtility::Lerp(m_retryInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_selectInfo.alpha = DEFAULT_FONT_ALPHA;
	m_titleInfo.alpha = DEFAULT_FONT_ALPHA;
	// �T�C�Y
	m_retryInfo.scale = UserUtility::Lerp(m_retryInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_selectInfo.scale = DEFAULT_FONT_SCALE;
	m_titleInfo.scale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// �Z���N�g��I�𒆂̃e�N�X�`���̏��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::CaseSelect()
{
	// �����x
	m_retryInfo.alpha = DEFAULT_FONT_ALPHA;
	m_selectInfo.alpha = UserUtility::Lerp(m_selectInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_titleInfo.alpha = DEFAULT_FONT_ALPHA;
	// �T�C�Y
	m_retryInfo.scale = DEFAULT_FONT_SCALE;
	m_selectInfo.scale = UserUtility::Lerp(m_selectInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_titleInfo.scale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// �^�C�g����I�𒆂̃e�N�X�`���̏��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::CaseTitle()
{
	// �����x
	m_retryInfo.alpha = DEFAULT_FONT_ALPHA;
	m_selectInfo.alpha = DEFAULT_FONT_ALPHA;
	m_titleInfo.alpha = UserUtility::Lerp(m_titleInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	// �T�C�Y
	m_retryInfo.scale = DEFAULT_FONT_SCALE;
	m_selectInfo.scale = DEFAULT_FONT_SCALE;
	m_titleInfo.scale = UserUtility::Lerp(m_titleInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
}
