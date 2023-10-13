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
	: m_rightAlpha{}		// �E���̓����x
	, m_leftAlpha{}			// �����̓����x
	, m_oneCoins{}			//   1�̈ʂ̃R�C����
	, m_tenCoins{}			//  10�̈ʂ̃R�C����
	, m_hanCoins{}			// 100�̈ʂ̃R�C����
	, m_moveY{}				// �㉺�ړ�
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜�̓o�^
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow", L"Resources/Textures/SELECT_INFO/RightArrow.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",  L"Resources/Textures/SELECT_INFO/LeftArrow.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin", L"Resources/Textures/SELECT_INFO/TotalCoins.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Number",	 L"Resources/Textures/Number.dds",				   device);
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
/// <param name="rightFlag">�E�L�[������������</param>
/// <param name="leftFlag">���L�[������������</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Update(const bool& rightFlag, const bool& leftFlag)
{
	// �t���O�ɂ���ē����x��ς���
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha  = !leftFlag  ? 0.5f : 1.0f;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
/// <param name="maxNum">�ő�̃X�e�[�W�ԍ�</param>
/// <returns>�Ȃ�</returns>
void SelectUI::Render(const int& selectNum, const int& maxNum)
{
	// ��ʔ䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;
	SimpleMath::Vector2 coinsScale = m_windowSize / FULL_SCREEN_SIZE / 2;

	//-------------------------------------------------------------------------------------//

	// ���l���R�C������\��
	m_system->GetDrawSprite()->DrawTexture(
		L"CenterCoin",
		SimpleMath::Vector2{ 0.0f ,0.0f } * scale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		coinsScale,
		SimpleMath::Vector2::Zero
	);

	DrawNumber(coinsScale);

	//-------------------------------------------------------------------------------------//

	if (selectNum != 0) // ������\��
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"LeftArrow",
			SimpleMath::Vector2{ 20.0f ,360.0f } * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_leftAlpha },
			scale,
			SimpleMath::Vector2::Zero
		);
	}
	if (selectNum != maxNum) // �E����\��
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"RightArrow",
			SimpleMath::Vector2{ 1650.0f,360.0f} * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_rightAlpha },
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

/// <summary>
/// �����̕`��
/// </summary>
/// <param name="scale">�e�N�X�`���̃X�P�[��</param>
/// <returns>�Ȃ�</returns>
void SelectUI::DrawNumber(SimpleMath::Vector2 scale)
{
	// ��ʃT�C�Y
	SimpleMath::Vector2 wScale = m_windowSize / FULL_SCREEN_SIZE;

	// �؂���ʒu�ݒ�
	RECT_U oneRec = { m_oneCoins * NUM_WIDTH, 0,m_oneCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U tenRec = { m_tenCoins * NUM_WIDTH, 0,m_tenCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U hanRec = { m_hanCoins * NUM_WIDTH, 0,m_hanCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };

	float positionY = 45.0f + sinf(m_moveY * MOVE_SPEED) * MOVE_WIDTH;

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 450.0f ,positionY } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		hanRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 500.0f ,positionY } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 550.0f ,positionY } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		oneRec
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
