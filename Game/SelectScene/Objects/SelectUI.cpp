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

// �R���X�g���N�^
SelectUI::SelectUI()
	: m_rightAlpha{}		// �E���̓����x
	, m_leftAlpha{}			// �����̓����x
	, m_oneCoins{}			//   1�̈ʂ̃R�C����
	, m_tenCoins{}			//  10�̈ʂ̃R�C����
	, m_hanCoins{}			// 100�̈ʂ̃R�C����
	, m_moveY{}				// �㉺�ړ�
	, is_pushToFlag{ false }// �X�y�[�X�摜�_�Ńt���O
{
}

// �f�X�g���N�^
SelectUI::~SelectUI()
{
	Finalize();
}

// �쐬�֐�
void SelectUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// �摜�̓o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number",		L"Resources/Textures/Number.dds");
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow",	L"Resources/Textures/SELECT_INFO/RightArrow.dds");
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",		L"Resources/Textures/SELECT_INFO/LeftArrow.dds");
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin",	L"Resources/Textures/SELECT_INFO/TotalCoins.dds");
	m_system->GetDrawSprite()->AddTextureData(L"PushSpace",		L"Resources/Textures/SELECT_INFO/SpaceToStart.dds");
	m_system->GetDrawSprite()->AddTextureData(L"OutsideFrame",	L"Resources/Textures/SELECT_INFO/OutFrame.dds");
}

// �X�V����
void SelectUI::Update(const bool& rightFlag, const bool& leftFlag)
{
	// �t���O�ɂ���ē����x��ς���
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha  = !leftFlag  ? 0.5f : 1.0f;
}

// �`�揈��
void SelectUI::Render(const float& fadeValue, const int& selectNum, const int& maxNum)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;
	SimpleMath::Vector2 _coinsScale = m_windowSize / FULL_SCREEN_SIZE / 1.5;

	// �O�g�t���[����`��
	DrawFrame(_rate);

	// ���l���R�C������\��
	m_system->GetDrawSprite()->DrawTexture(
		L"CenterCoin",
		SimpleMath::Vector2::Zero,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);

	// ������`��
	DrawNumber(_rate, _coinsScale);

	// ����`��
	DrawArrow(_rate, selectNum, maxNum);

	// �t�F�[�h���͏������Ȃ�
	if (fadeValue >= 0.7f) return;

	// �v�b�V���摜��\��
	m_system->GetDrawSprite()->DrawTexture(
		L"PushSpace",
		SimpleMath::Vector2{ (FULL_SCREEN_SIZE.x - PUSH_SIZE.x) / 2, FULL_SCREEN_SIZE.y - PUSH_SIZE.y } * _rate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);
}

// �I������
void SelectUI::Finalize()
{
	m_system.reset();
}

// ���`��
void SelectUI::DrawArrow(SimpleMath::Vector2 windowRate, const int& selectNum, const int& maxNum)
{
	if (selectNum != 0) // ������\��
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"LeftArrow",
			SimpleMath::Vector2{ 20.0f ,360.0f } * windowRate,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_leftAlpha },
			windowRate,
			SimpleMath::Vector2::Zero
		);
	}
	if (selectNum != maxNum) // �E����\��
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"RightArrow",
			SimpleMath::Vector2{ 1650.0f,360.0f } * windowRate,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_rightAlpha },
			windowRate,
			SimpleMath::Vector2::Zero
		);
	}
}

// �����̕`��
void SelectUI::DrawNumber(SimpleMath::Vector2 windowRate, SimpleMath::Vector2 texScale)
{
	// �؂���ʒu�ݒ�
	RECT_U _oneRec = { m_oneCoins * NUM_WIDTH, 0,m_oneCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U _tenRec = { m_tenCoins * NUM_WIDTH, 0,m_tenCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U _hanRec = { m_hanCoins * NUM_WIDTH, 0,m_hanCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };

	float _positionY = 15.0f + sinf(m_moveY * MOVE_SPEED) * MOVE_WIDTH;

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 500.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_hanRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 550.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 600.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_oneRec
	);
}

// �O�g�t���[��
void SelectUI::DrawFrame(SimpleMath::Vector2 windowRate)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"OutsideFrame",
		SimpleMath::Vector2::Zero,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		windowRate,
		SimpleMath::Vector2::Zero
	);
}

// �R�C�������e���ɕϊ�
void SelectUI::SetAllCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
	m_hanCoins = totalCoinNum / 100;
}