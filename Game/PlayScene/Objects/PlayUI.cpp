/*
 *	@File	PlayUI.cpp
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e���}�l�[�W��
#include "../../../Libraries/SystemManager/SystemManager.h"

// ���[�U�[���[�e�B���e�B
#include "../../../Libraries/UserUtility.h"

#include "PlayUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
PlayUI::PlayUI(const SimpleMath::Vector2& windowSize)
	: m_system{}								// �V�X�e���}�l�[�W��
	, m_windowSize{ windowSize }				// �E�B���h�E�T�C�Y
	, m_gameTimer{0}							// �Q�[���̃^�C�}�[
    , m_oneSecPos{SimpleMath::Vector2::Zero}	// 1�b�̍��W
    , m_tenSecPos{SimpleMath::Vector2::Zero}	// 10�b�̍��W
	, m_countDownPos{SimpleMath::Vector2::Zero}	// �R�J�E���g�_�E���̍��W
	, m_underFontPos{SimpleMath::Vector2::Zero}	// ���̕������[���̍��W
	, is_effectFlag{false}						// �G�t�F�N�g�̕\���t���O
	, is_helpFlag{false}						// �w���v��ʂ̃t���O
{
	m_windowSize = windowSize;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayUI::~PlayUI()
{
	Finalize();
	m_system.reset();
}

/// <summary>
/// �쐬����
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="context">�R���e�L�X�g�|�C���^</param>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Create(std::shared_ptr<SystemManager> system ,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �V�X�e��
	m_system = system;

	// �摜�̐ݒ�
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Death",  L"Resources/Textures/DeathEffect.dds", device);

	// �w���v���
	m_system->GetDrawSprite()->AddTextureData(L"Help",	   L"Resources/Textures/PLAY_HELP/Help.dds",      device);
	m_system->GetDrawSprite()->AddTextureData(L"HelpBack", L"Resources/Textures/PLAY_HELP/HelpBack.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp", L"Resources/Textures/PLAY_HELP/OpenHelp.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"GameStart",L"Resources/Textures/PLAY_HELP/GameStart.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderBack",L"Resources/Textures/PLAY_HELP/UnderBack.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderFont",L"Resources/Textures/PLAY_HELP/UnderFont.dds", device);

	// �䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// �X�v���C�g�̈ʒu���v�Z
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * scale.x , 80.0f * scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * scale.x ,m_countDownPos.y };
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * scale.x ,m_countDownPos.y };

	// �����t���O��؂�
	is_effectFlag = false;
	// �w���v�t���O��؂�
	is_helpFlag = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timelimit">�c�莞��</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Update(const float& timelimit)
{
	m_gameTimer = static_cast<int>(timelimit);

	// �䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// �ړ�����
	m_underFontPos.x -= UNDER_SPEED * scale.x;

	if (m_underFontPos.x < -FULL_SCREEN_SIZE.x * 2 * scale.x)
	{
		m_underFontPos.x = FULL_SCREEN_SIZE.x;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Render()
{
    // �䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// �����G�t�F�N�g
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
	}

	// �^�C�}�[�̕`��
	RenderTimer(scale);

	// �w���v�t���O(�������)
	if (is_helpFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"HelpBack",                       // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"Help",                           // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"OpenHelp",                       // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);

		//-------------------------------------------------------------------------------------//
		// ��ʉ��̃t�H���g
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderBack",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			scale,
			SimpleMath::Vector2::Zero
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderFont",
			m_underFontPos,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			scale,
			SimpleMath::Vector2::Zero,
			{ 0,0,static_cast<LONG>(FULL_SCREEN_SIZE.x * 2),static_cast<LONG>(FULL_SCREEN_SIZE.y) }
		);
	}
}

/// <summary>
/// �J�n�̃J�E���g�_�E��
/// </summary>
/// <param name="countDown">�J�E���g�_�E��</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderCountDown(const float& countDown)
{
	// �䗦���v�Z
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",                      // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
		return;
	}

	int num = static_cast<int>(countDown) / 60;
	RECT_U countRec = { num * NUM_SIZE, 0, num * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_countDownPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		countRec
	);
}

/// <summary>
/// �^�C�}�[�̕`��
/// </summary>
/// <param name="scale">�g�嗦</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderTimer(SimpleMath::Vector2 scale)
{
	// 1�̈ʂɕϊ�
	int oneSec = m_gameTimer / 60 % 10;
	int tenSec = m_gameTimer / 60 / 10 % 10;

	// �؂���ʒu�ݒ�
	RECT_U oneRec = { oneSec * NUM_SIZE, 0, oneSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U tenRec = { tenSec * NUM_SIZE, 0, tenSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_tenSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_oneSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		oneRec
	);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Finalize()
{
}
