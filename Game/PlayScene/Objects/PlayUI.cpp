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
PlayUI::PlayUI(const SimpleMath::Vector2& windowSize):
	m_system{},
	m_windowSize{ windowSize },
	m_timeLimit{0},
    m_oneSecPos{SimpleMath::Vector2::Zero},
    m_tenSecPos{SimpleMath::Vector2::Zero},
	m_countDownPos{SimpleMath::Vector2::Zero},
	is_effectFlag{false},
	is_helpFlag{false}
{
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
	m_system->GetDrawSprite()->AddTextureData(L"Death", L"Resources/Textures/DeathEffect.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Help", L"Resources/Textures/PLAY_HELP/Help.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"HelpBack", L"Resources/Textures/PLAY_HELP/HelpBack.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp", L"Resources/Textures/PLAY_HELP/OpenHelp.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"GameStart", L"Resources/Textures/PLAY_HELP/GameStart.dds", device);

	// �䗦���v�Z
	SimpleMath::Vector2 span = m_windowSize / FULL_SCREEN_SIZE;

	// �X�v���C�g�̈ʒu���v�Z
	m_oneSecPos	   = { 1010.0f * span.x, 80.0f * span.y };
	m_tenSecPos	   = {  910.0f * span.x, 80.0f * span.y };
	m_countDownPos = {  960.0f * span.x, 80.0f * span.y };

    // ���W�␳
    m_oneSecPos.x	 -= NUMBER_SPRITE_SIZE / 2 * span.x;
    m_tenSecPos.x	 -= NUMBER_SPRITE_SIZE / 2 * span.x;
	m_countDownPos.x -= NUMBER_SPRITE_SIZE / 2 * span.x;

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
	m_timeLimit = static_cast<int>(timelimit);
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

    // �b�����v�Z
    int oneSec = m_timeLimit / 60;

    // �ꌅ�ڂ̐�����\��
    RenderDigit(oneSec % 10, m_oneSecPos, scale, NUMBER_SPRITE_SIZE, NUMBER_SPRITE_SIZE);

    // �\�̌��̐�����\��
    RenderDigit((oneSec / 10) % 10, m_tenSecPos, scale, NUMBER_SPRITE_SIZE, NUMBER_SPRITE_SIZE);

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

	// �摜�\��
	RenderDigit(
		static_cast<int>(countDown / 60),
		m_countDownPos,
		scale,
		NUMBER_SPRITE_SIZE,
		NUMBER_SPRITE_SIZE
	);
}

/// <summary>
/// ������`�悷��
/// </summary>
/// <param name="digit">�`�悷�鐔��</param>
/// <param name="position">���W</param>
/// <param name="scale">�g�嗦</param>
/// <param name="digitWidth">�����̕�</param>
/// <param name="digitHeight">�����̍���</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderDigit(int digit, const SimpleMath::Vector2& position, SimpleMath::Vector2 scale, int digitWidth, int digitHeight)
{
	// �X�v���C�g�̈ʒu���v�Z
	float spritePosX = position.x * scale.x;
	float spritePosY = position.y * scale.y;

	// �X�v���C�g�̒��S�ʒu���v�Z
	SimpleMath::Vector2 center = { spritePosX * scale.x / 2.0f, spritePosY * scale.y / 2.0f };

	// �؂���ʒu�̐ݒ�
	RECT_U rect;

	// �؂���J�n�ʒu��ݒ�(��)
	rect.left = digit * digitWidth;

	// �؂���I���ʒu��ݒ�(��)
	rect.right = rect.left + digitWidth;

	// �摜�c����ݒ�
	rect.bottom = digitHeight;

	// �����\��
	m_system->GetDrawSprite()->DrawTexture(
		L"Number",                         // �o�^�L�[
		position + center,                 // ���W
		{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
		scale,                             // �g�嗦
		center,                            // ���S�ʒu
		rect                               // �؂���ʒu
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
