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
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
SelectUI::SelectUI()
	: m_timer{}				// �^�C�}�[
	, m_rightAlpha{}		// �E���̓����x
	, m_leftAlpha{}			// �����̓����x
	, m_oneCoins{}			//   1�̈ʂ̃R�C����
	, m_tenCoins{}			//  10�̈ʂ̃R�C����
	, m_hanCoins{}			// 100�̈ʂ̃R�C����
	, m_moveY{}				// �㉺�ړ�
	, is_pushToFlag{ false }// �X�y�[�X�摜�_�Ńt���O
{
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
/// �쐬�֐�
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <param name="windowSize">�E�B���h�E�T�C�Y</param>
/// <param name="fullSize">�t���X�N���[���T�C�Y</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// �摜�̓o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number",		L"Resources/Textures/Number.dds",					device);
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow",	L"Resources/Textures/SELECT_INFO/RightArrow.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",		L"Resources/Textures/SELECT_INFO/LeftArrow.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin",	L"Resources/Textures/SELECT_INFO/TotalCoins.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"PushSpace",		L"Resources/Textures/SELECT_INFO/SpaceToStart.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"OutsideFrame",	L"Resources/Textures/SELECT_INFO/OutFrame.dds",		device);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="rightFlag">�E�L�[������������</param>
/// <param name="leftFlag">���L�[������������</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Update(const float& timer, const bool& rightFlag, const bool& leftFlag)
{
	m_timer = timer;

	// �t���O�ɂ���ē����x��ς���
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha  = !leftFlag  ? 0.5f : 1.0f;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="fadeValue">�t�F�[�h�̒l</param>
/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
/// <param name="maxNum">�ő�̃X�e�[�W�ԍ�</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Render(const float& fadeValue, const int& selectNum, const int& maxNum)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize;
	SimpleMath::Vector2 _coinsRate = m_windowSize / m_fullScreenSize / 1.5;

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
	DrawNumber(_rate, _coinsRate);

	// ����`��
	DrawArrow(_rate, selectNum, maxNum);

	// �t�F�[�h���͏������Ȃ�
	if (fadeValue >= 0.7f) return;

	// �v�b�V���摜��\��
	m_system->GetDrawSprite()->DrawTexture(
		L"PushSpace",
		SimpleMath::Vector2{ (m_fullScreenSize.x - PUSH_SIZE.x) / 2, m_fullScreenSize.y - PUSH_SIZE.y } * _rate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);
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

/// <summary>
/// ����`�悷��
/// </summary>
/// <param name="windowRate">�E�B���h�E�䗦</param>
/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
/// <param name="maxNum">�X�e�[�W�ő吔</param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �����̕`��
/// </summary>
/// <param name="windowRate">�E�B���h�E�䗦</param>
/// <param name="texScale">�e�N�X�`���̃X�P�[��</param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �O�g�t���[��
/// </summary>
/// <param name="windowRate">�E�B���h�E�䗦</param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// ���v�R�C�����̌v�Z
/// </summary>
/// <param name="totalCoinNum">�R�C����</param>
/// <returns>�Ȃ�</returns>
void SelectUI::SetAllCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
	m_hanCoins = totalCoinNum / 100;
}