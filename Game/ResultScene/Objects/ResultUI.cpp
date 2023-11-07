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

// �R���X�g���N�^
ResultUI::ResultUI()
	: m_selectingScene{ NEXT }	// ���ݑI�𒆂̃V�[��
	, m_oneCoins{}				//  1�̈ʂ̃R�C����
	, m_tenCoins{}				// 10�̈ʂ̃R�C����
	, m_oneTime{}				//  1�̈ʂ̃N���A�^�C��
	, m_tenTime{}				// 10�̈ʂ̃N���A�^�C��
	, m_texInfo{}				// �t�H���g�̏��
	, m_rect{}					// �؂���ʒu
	, m_coinNum{ 0 }			// �R�C���̖���
{
}

// �f�X�g���N�^
ResultUI::~ResultUI()
{
	Finalize();
}

// ����������
void ResultUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number",L"Resources/Textures/Number.dds");
	m_system->GetDrawSprite()->AddTextureData(L"RFont", L"Resources/Textures/RESULT_INFO/ResultFonts.dds");
	m_system->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/RESULT_INFO/ResultBack.dds");
	m_system->GetDrawSprite()->AddTextureData(L"PACK",  L"Resources/Textures/RESULT_INFO/ResultSelectPack.dds");

	// �����̍��W�̏�����
	float _commonY = FULL_SCREEN_SIZE.y - static_cast<float>(NUM_SIZE * 2);
	m_texInfo[NEXT].pos = SimpleMath::Vector2{FONT_WIDTH * 0.9f, _commonY};
	m_texInfo[RETRY].pos = SimpleMath::Vector2{ m_texInfo[NEXT].pos.x + FONT_WIDTH, _commonY};
	m_texInfo[SELECT].pos = SimpleMath::Vector2{ m_texInfo[RETRY].pos.x + FONT_WIDTH, _commonY};
	// �摜�̐؂���ʒu
	m_rect[NEXT] = {0L,0L,512L, 128L};
	m_rect[RETRY] = {0L,128L,512L, 256L};
	m_rect[SELECT] = {0L,256L,512L, 384L};
}

// �X�V����
void ResultUI::Update(const int& clearTime)
{
	// �N���A�^�C����ۑ�
	m_oneTime = clearTime % 10;
	m_tenTime = clearTime / 10 % 10;

	// �A���t�@�l�ƃX�P�[���̕ύX
	UpdateTexture(m_selectingScene);
}

// �`�揈��
void ResultUI::Render(const bool& drawSelect)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;

	// ���C�A�E�g�̕`��
	DrawLayout(_rate);

	// ������`��
	DrawNumber(SimpleMath::Vector2::One, _rate);

	// �t�F�[�h���͏������Ȃ�
	if (not drawSelect) return;

	// �V�[���I�𕶎�(�l�N�X�g�A���g���C�A�Z���N�g)
	DrawFonts(_rate);
}

// �I������
void ResultUI::Finalize()
{
	m_system.reset();
}

// �����̕`��
void ResultUI::DrawNumber(SimpleMath::Vector2 texScale, const SimpleMath::Vector2& windowRate)
{
	// �N���A����
	// �؂���ʒu�ݒ�
	RECT_U _oneRec = { m_oneTime * NUM_SIZE, 0,m_oneTime * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U _tenRec = { m_tenTime * NUM_SIZE, 0,m_tenTime * NUM_SIZE + NUM_SIZE, NUM_SIZE };

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

	// �R�C���̐�
	// �؂���ʒu�ݒ�
	_oneRec = { m_oneCoins * NUM_SIZE, 0,m_oneCoins * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	_tenRec = { m_tenCoins * NUM_SIZE, 0,m_tenCoins * NUM_SIZE + NUM_SIZE, NUM_SIZE };

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

// ���C�A�E�g�̕`��
void ResultUI::DrawLayout(const SimpleMath::Vector2& windowRate)
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

// �t�H���g�̕`��
void ResultUI::DrawFonts(const SimpleMath::Vector2& windowRate)
{
	for (int i = 0; i < SELECTION::LENGTH; ++i)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"PACK",
			m_texInfo[i].pos * windowRate,
			{ 1.0f, 1.0f, 1.0f, m_texInfo[i].alpha },
			IMAGE_RATE * windowRate * m_texInfo[i].scale,
			{ FONT_WIDTH / 2,FONT_HEIGHT / 2 },
			m_rect[i]
		);
	}
}

// �e�N�X�`���̕ύX
void ResultUI::UpdateTexture(SELECTION which)
{
	// �����x�ƃX�P�[���̕ύX
	for (int i = 0; i < SELECTION::LENGTH; ++i)
	{
		if (i == which)
		{
			m_texInfo[which].alpha = UserUtility::Lerp(m_texInfo[which].alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
			m_texInfo[which].scale = UserUtility::Lerp(m_texInfo[which].scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
		}
		else
		{
			m_texInfo[i].alpha = DEFAULT_FONT_ALPHA;
			m_texInfo[i].scale = DEFAULT_FONT_SCALE;
		}
	}
}

// ���v�R�C�����̐ݒ�
void ResultUI::SetCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
}