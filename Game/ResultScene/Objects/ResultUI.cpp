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
 /// <param name="system">�V�X�e���}�l�[�W��</param>
 /// <param name="context">�R���e�L�X�g�|�C���^</param>
 /// <param name="device">�f�o�C�X�|�C���^</param>
 /// <returns>�Ȃ�</returns>
ResultUI::ResultUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
	: m_selectingScene{ RETRY }	// ���ݑI�𒆂̃V�[��
	, m_oneCoins{}				//  1�̈ʂ̃R�C����
	, m_tenCoins{}				// 10�̈ʂ̃R�C����
	, m_oneTime{}				//  1�̈ʂ̃N���A�^�C��
	, m_tenTime{}				// 10�̈ʂ̃N���A�^�C��
	//-------------------------------------------------------------------------------------//
	,m_retryPos{}				// ���g���C�e�L�X�g�̈ʒu
	,m_retryAlpha{}				// �����x
	,m_retryScale{}				// �傫��
	//-------------------------------------------------------------------------------------//
	,m_selectPos{}				// �Z���N�g�e�L�X�g�̈ʒu
	,m_selectAlpha{}			// �����x
	,m_selectScale{}			// �傫��
	//-------------------------------------------------------------------------------------//
	,m_titlePos{}				// �^�C�g���e�L�X�g�̈ʒu
	,m_titleAlpha{}				// �����x
	,m_titleScale{}				// �傫��
	//-------------------------------------------------------------------------------------//
	,m_coinNum{ 0 }				// ����
	//-------------------------------------------------------------------------------------//
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"RETRY",  L"Resources/Textures/FONT/RETRY.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"TITLE",  L"Resources/Textures/FONT/TITLE.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"RFont",  L"Resources/Textures/ResultFonts.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BLIND",  L"Resources/Textures/ResultBack.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds",      device);
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
/// ����������
/// </summary>
/// <param name="windowSize">��ʃT�C�Y</param>
/// <returns>�Ȃ�</returns>
void ResultUI::Initialize(const SimpleMath::Vector2& windowSize)
{
	m_windowSize = windowSize;

	SimpleMath::Vector2 rate = m_windowSize / FULL_SCREEN_SIZE;

	// �����̍��W�̏�����
	m_retryPos  = SimpleMath::Vector2{ FONT_WIDTH / 2, FULL_SCREEN_SIZE.y - 150.0f };
	m_selectPos = SimpleMath::Vector2{ m_retryPos.x + FONT_WIDTH, FULL_SCREEN_SIZE.y - 150.0f };
	m_titlePos  = SimpleMath::Vector2{ m_selectPos.x + FONT_WIDTH, FULL_SCREEN_SIZE.y - 150.0f };
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">����</param>
/// <param name="clearTime">�N���A�^�C��</param>
/// <returns>�Ȃ�</returns>
void ResultUI::Update(const float& timer, const int& clearTime)
{
	m_timer = timer;

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
/// <param name = "��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::Render()
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	//-------------------------------------------------------------------------------------//

	// ��ʂ𔖈Â�����
	m_system->GetDrawSprite()->DrawTexture(
		L"BLIND",									// �o�^�L�[
		SimpleMath::Vector2::Zero,					// ���W
		{ 1.0f,1.0f,1.0f,0.3f },					// �F
		1.0f * scale,								// �g�嗦
		SimpleMath::Vector2::Zero					// ���S�ʒu
	);

	// ���̕���
	m_system->GetDrawSprite()->DrawTexture(
		L"RFont",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		DEFAULT_RATE * scale,
		SimpleMath::Vector2::Zero
	);

	// ������`��
	DrawNumber(SimpleMath::Vector2::One);

	// �V�[���I�𕶎�(���g���C�A�Z���N�g�A�^�C�g��)
	m_system->GetDrawSprite()->DrawTexture(
		L"RETRY",
		m_retryPos * scale,
		{ 1.0f, 1.0f, 1.0f, m_retryAlpha },
		IMAGE_RATE * scale * m_retryScale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"SELECT",
		m_selectPos * scale,
		{ 1.0f, 1.0f, 1.0f, m_selectAlpha },
		IMAGE_RATE * scale * m_selectScale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"TITLE",
		m_titlePos * scale,
		{ 1.0f, 1.0f, 1.0f, m_titleAlpha },
		IMAGE_RATE * scale * m_titleScale,
		SimpleMath::Vector2::Zero
	);
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
/// <param name="scale">�e�N�X�`���̃X�P�[��</param>
/// <returns>�Ȃ�</returns>
void ResultUI::DrawNumber(SimpleMath::Vector2 scale)
{
	// ��ʃT�C�Y
	SimpleMath::Vector2 wScale = m_windowSize / FULL_SCREEN_SIZE;

	//-------------------------------------------------------------------------------------//
	// �N���A����
	// �؂���ʒu�ݒ�
	RECT_U oneRec = { m_oneTime * 100, 0,m_oneTime * 100 + 100, 100 };
	RECT_U tenRec = { m_tenTime * 100, 0,m_tenTime * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,290.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,290.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	//-------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------//
	// �R�C���̐�
	// �؂���ʒu�ݒ�
	oneRec = { m_oneCoins * 100, 0,m_oneCoins * 100 + 100, 100 };
	tenRec = { m_tenCoins * 100, 0,m_tenCoins * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,760.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,760.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		tenRec
	);
	//-------------------------------------------------------------------------------------//
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
	m_retryAlpha = UserUtility::Lerp(m_retryAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_selectAlpha = DEFAULT_FONT_ALPHA;
	m_titleAlpha = DEFAULT_FONT_ALPHA;
	// �T�C�Y
	m_retryScale = UserUtility::Lerp(m_retryScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_selectScale = DEFAULT_FONT_SCALE;
	m_titleScale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// �Z���N�g��I�𒆂̃e�N�X�`���̏��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::CaseSelect()
{
	// �����x
	m_retryAlpha = DEFAULT_FONT_ALPHA;
	m_selectAlpha = UserUtility::Lerp(m_selectAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_titleAlpha = DEFAULT_FONT_ALPHA;
	// �T�C�Y
	m_retryScale = DEFAULT_FONT_SCALE;
	m_selectScale = UserUtility::Lerp(m_selectScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_titleScale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// �^�C�g����I�𒆂̃e�N�X�`���̏��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultUI::CaseTitle()
{
	// �����x
	m_retryAlpha = DEFAULT_FONT_ALPHA;
	m_selectAlpha = DEFAULT_FONT_ALPHA;
	m_titleAlpha = UserUtility::Lerp(m_titleAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	// �T�C�Y
	m_retryScale = DEFAULT_FONT_SCALE;
	m_selectScale = DEFAULT_FONT_SCALE;
	m_titleScale = UserUtility::Lerp(m_titleScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
}
