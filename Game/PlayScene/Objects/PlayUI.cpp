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
	, m_sunPos{SimpleMath::Vector2::Zero}		// ���z�̉摜�̍��W
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
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Create(const std::shared_ptr<SystemManager>& system ,ID3D11Device1* device)
{
	// �V�X�e��
	m_system = system;

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number",			// �����X�v���C�g
		L"Resources/Textures/Number.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"GameStart",			// �X�^�[�g���}
		L"Resources/Textures/PLAY_COMMON/GameStart.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Death",				// ���S�G�t�F�N�g
		L"Resources/Textures/PLAY_COMMON/DeathEffect.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Sun",				// ���v���[�^�[
		L"Resources/Textures/PLAY_COMMON/sun.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Help",				// �w���v�\��
		L"Resources/Textures/PLAY_HELP/Help.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"HelpBack",			// �w���v���̔w�i�Ó]
		L"Resources/Textures/PLAY_HELP/HelpBack.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp",			// �w���v���J���\��
		L"Resources/Textures/PLAY_HELP/OpenHelp.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"UnderFont",			// ���̃e�L�X�g
		L"Resources/Textures/PLAY_HELP/UnderFont.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"UnderBack",			// ���̃e�L�X�g�̃��C��
		L"Resources/Textures/PLAY_HELP/UnderBack.dds", device);

	// �䗦���v�Z
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// �X�v���C�g�̈ʒu���v�Z
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * _scale.x , 80.0f * _scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y };
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y };
	m_sunPos = { (SUN_SIZE.x * _scale.x) / 2, SUN_SIZE.y * _scale.y };

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
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// ���z�̉�]���Z�b�g
	m_system->GetDrawSprite()->CreateRotation(L"Sun", static_cast<float>(MAX_LIMITS - m_gameTimer) * ROT_SPEED);

	// ���z�̈ړ�����
	m_sunPos.x += m_gameTimer != 0 ? SUN_MOVE_SPEED * _scale.x : 0.0f;

	// �t�H���g�̈ړ�����
	m_underFontPos.x -= UNDER_SPEED * _scale.x;

	if (m_underFontPos.x < -FULL_SCREEN_SIZE.x * 2 * _scale.x)
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
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// �����G�t�F�N�g
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			_scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
	}

	// ���z�A�C�R���̕`��
	RenderSunny(_scale);

	// �^�C�}�[�̕`��
	RenderTimer(_scale);

	//-------------------------------------------------------------------------------------//
	// �w���v���̕\��
	if (is_helpFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"HelpBack",                       // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			_scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"Help",                           // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			_scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
	}
	//-------------------------------------------------------------------------------------//
	// �ʏ�̕\��
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"OpenHelp",
			SimpleMath::Vector2{ m_windowSize.x - HELP_WIDTH * _scale.x, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			{ 0,0,360,120 }
		);

		// ��ʉ��̃t�H���g
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderBack",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderFont",
			m_underFontPos,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			// �摜�̃T�C�Y
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
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",                      // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			_scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
		return;
	}

	int _num = static_cast<int>(countDown) / 60;
	RECT_U _countRec = { _num * NUM_SIZE, 0, _num * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_countDownPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		_scale,
		SimpleMath::Vector2::Zero,
		_countRec
	);
}

/// <summary>
/// �^�C�}�[�̕`��
/// </summary>
/// <param name="scale">�g�嗦</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderTimer(SimpleMath::Vector2 scale)
{
	// �ꌅ�ɕϊ�
	int _oneSec = m_gameTimer / 60 % 10;
	int _tenSec = m_gameTimer / 60 / 10 % 10;

	// �؂���ʒu�ݒ�
	RECT_U _oneRec = { _oneSec * NUM_SIZE, 0, _oneSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U _tenRec = { _tenSec * NUM_SIZE, 0, _tenSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_tenSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_oneSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_oneRec
	);
}

/// <summary>
/// ���z�̕`��
/// </summary>
/// <param name="scale">�摜�̊g�嗦</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderSunny(SimpleMath::Vector2 scale)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"Sun",
		m_sunPos,
		{ 1.0f,1.0f,1.0f,1.0f },
		scale,
		SimpleMath::Vector2{ SUN_SIZE / 2},
		{ 0,0,100,100 }
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
