/*
 *	@File	PlayUI.cpp
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "ShowKey.h"
#include "Libraries/UserUtility.h"
#include "PlayUI.h"

// �R���X�g���N�^
PlayUI::PlayUI()
	: m_system{}								// �V�X�e���}�l�[�W��
	, m_windowSize{}							// �E�B���h�E�T�C�Y
	, m_gameTimer{0}							// �Q�[���̃^�C�}�[
    , m_oneSecPos{SimpleMath::Vector2::Zero}	// 1�b�̍��W
    , m_tenSecPos{SimpleMath::Vector2::Zero}	// 10�b�̍��W
	, m_countDownPos{SimpleMath::Vector2::Zero}	// �R�J�E���g�_�E���̍��W
	, m_countDownEnds{1.0f}						// �J�E���g�_�E���������鎞��
	, m_moveTexPos{SimpleMath::Vector2::Zero}	// �摜�ړ��p���W
	, m_arrowPos{SimpleMath::Vector2::Zero}		// �I�����ډ������W
	, m_helpPoses{}								// �w���v�̍��W
	, m_targetArrow{}							// �I���ɉ��������W
	, m_pageNum{1}								// �w���v�̃y�[�W�ԍ�
	, m_transNum{0}								// �J�ڔԍ�
	, m_sunPos{SimpleMath::Vector2::Zero}		// ���z�̉摜�̍��W
	, is_transFlag{ false }						// �J�ډ�ʃt���O
	, is_effectFlag{false}						// �G�t�F�N�g�̕\���t���O
	, is_helpFlag{false}						// �w���v��ʂ̃t���O
{

}

// �f�X�g���N�^
PlayUI::~PlayUI()
{
	Finalize();
	m_system.reset();
	m_showKey.reset();
}

// �쐬�֐�
void PlayUI::Create(const std::shared_ptr<SystemManager>& system ,const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// �L�[�\���̍쐬
	m_showKey = std::make_unique<ShowKey>();

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number",			// �����X�v���C�g
		L"Resources/Textures/Number.dds");

	m_system->GetDrawSprite()->AddTextureData(L"GameStart",			// �X�^�[�g���}
		L"Resources/Textures/PLAY_COMMON/GameStart.dds");

	m_system->GetDrawSprite()->AddTextureData(L"Death",				// ���S�G�t�F�N�g
		L"Resources/Textures/PLAY_COMMON/DeathEffect.dds");

	m_system->GetDrawSprite()->AddTextureData(L"Sun",				// ���v���[�^�[
		L"Resources/Textures/PLAY_COMMON/Sun.dds");

	m_system->GetDrawSprite()->AddTextureData(L"HelpBack",			// �w���v���̔w�i�Ó]
		L"Resources/Textures/PLAY_HELP/HelpBack.dds");

	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp",			// �w���v���J���\��
		L"Resources/Textures/PLAY_HELP/OpenHelp.dds");

	m_system->GetDrawSprite()->AddTextureData(L"HowToPlay",			// �w���v�\��
		L"Resources/Textures/PLAY_HELP/Help.dds");

	m_system->GetDrawSprite()->AddTextureData(L"BlockInfo",			// �u���b�N���
		L"Resources/Textures/PLAY_HELP/BlockInfo.dds");

	m_system->GetDrawSprite()->AddTextureData(L"GoScenes",			// �V�[���Z���N�g
		L"Resources/Textures/PLAY_HELP/GoScenes.dds");

	m_system->GetDrawSprite()->AddTextureData(L"SelectArrow",		// �I���̖��
		L"Resources/Textures/PLAY_HELP/SelectArrow.dds");

	// �䗦���v�Z
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// �X�v���C�g�̈ʒu���v�Z
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * _scale.x , 80.0f * _scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y * _scale.y};
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y * _scale.y};

	m_sunPos = { (SUN_SIZE.x * _scale.x) / 2, SUN_SIZE.y * _scale.y };

	// �����W
	m_targetArrow[RETRY]  = 358.0f;
	m_targetArrow[SELECT] = 500.0f;
	m_targetArrow[TITLE]  = 642.0f;
	m_targetArrow[EXIT]   = 840.0f;

	// �w���v���W
	m_moveTexPos = SimpleMath::Vector2::Zero;
	m_pageNum = 1;
	m_helpPoses.emplace(L"HowToPlay", HelpPosition{ SimpleMath::Vector2::Zero ,
		SimpleMath::Vector2::Zero });
	m_helpPoses.emplace(L"BlockInfo", HelpPosition{ SimpleMath::Vector2{ FULL_SCREEN_SIZE.x, 0.0f},
		SimpleMath::Vector2{ FULL_SCREEN_SIZE.x, 0.0f} });
	m_helpPoses.emplace(L"GoScenes", HelpPosition{ SimpleMath::Vector2{ FULL_SCREEN_SIZE.x * 2, 0.0f},
		SimpleMath::Vector2{ FULL_SCREEN_SIZE.x * 2, 0.0f} });

	// �����W
	m_arrowPos.initPos = m_arrowPos.nowPos =
	{ m_helpPoses[L"GoScenes"].initPos.x + (FULL_SCREEN_SIZE.x / 3), m_targetArrow[RETRY]};

	// �����t���O��؂�
	is_effectFlag = false;
	// �w���v�t���O��؂�
	is_helpFlag = false;
	// �J�ډ�ʂł͂Ȃ�
	is_transFlag = false;
}

// �X�V����
void PlayUI::Update(const float& timelimit)
{
	m_gameTimer = static_cast<int>(timelimit);

	// �䗦���v�Z
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	m_showKey->Update();

	// ���z�̉�]���Z�b�g
	m_system->GetDrawSprite()->CreateRotation(L"Sun", static_cast<float>(MAX_LIMITS - m_gameTimer) * ROT_SPEED);

	// ���z�̈ړ�����
	m_sunPos.x += m_gameTimer != 0 ? SUN_MOVE_SPEED * _scale.x : 0.0f;

	// �ύX�����Z�b�g
	if (!is_helpFlag)
	{
		m_pageNum = 1;
		m_moveTexPos = SimpleMath::Vector2::Zero;
		m_transNum = RETRY;
		for (auto& pos : m_helpPoses)
		{
			// ���݂̍��W�A�������W
			MovePositions(&pos.second.nowPos, pos.second.initPos);
		}
		MovePositions(&m_arrowPos.nowPos, m_arrowPos.initPos);
	}
}

// �y�[�W���߂���
void PlayUI::UpdatePage(const bool& leftArrow, const bool& rightArrow)
{
	// �ړ��ʂ��v�Z
	if (leftArrow)
	{
		m_moveTexPos.x += FULL_SCREEN_SIZE.x;
		m_pageNum--;
	}
	if (rightArrow)
	{
		m_moveTexPos.x -= FULL_SCREEN_SIZE.x;
		m_pageNum++;
	}

	// �N�����v����
	m_moveTexPos.x = UserUtility::Clamp(m_moveTexPos.x, -FULL_SCREEN_SIZE.x * (MAX_PAGE - 1), 0.0f);
	m_pageNum = UserUtility::Clamp(m_pageNum, 1, MAX_PAGE);

	// �J�ډ�ʂȂ�True�ɂ���
	is_transFlag = static_cast<int>(m_pageNum) == 3 ? true : false;

	for (auto& pos : m_helpPoses)
	{
		// ���݂̍��W�A�������W�{�ړ���
		MovePositions(&pos.second.nowPos,pos.second.initPos + m_moveTexPos);
	}

	// X���݂̂̈ړ�
	MovePositions(&m_arrowPos.nowPos,
		SimpleMath::Vector2{ m_arrowPos.initPos.x, m_arrowPos.nowPos.y } + m_moveTexPos);
}

// �J�ڐ�̑I��
void PlayUI::UpdateTransition(const bool& upArrow, const bool& downArrow)
{
	if (upArrow)
	{
		m_transNum = m_transNum != RETRY ? m_transNum - 1 : EXIT;
	}
	if (downArrow)
	{
		m_transNum = m_transNum != EXIT ? m_transNum + 1 : RETRY;
	}

	// �w����W�Ɉړ�
	m_arrowPos.nowPos.y = UserUtility::Lerp(m_arrowPos.nowPos.y, m_targetArrow[m_transNum], 0.1f);
}

// �`��֐�
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
			_scale,                            // �g�嗦
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
			_scale,                            // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);

		// �y�[�W�̕`��
		RenderHelpPage(_scale);

		// �A���[
		if (is_transFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SelectArrow",
				m_arrowPos.nowPos * _scale,
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				 _scale,
				ARROW_SIZE / 2,
				RECT_U(0L, 0L, static_cast<LONG>(ARROW_SIZE.x), static_cast<LONG>(ARROW_SIZE.y))
			);
		}
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
			{ 0L,0L,360L,120L }
		);

		// �L�[���͂�\��
		m_showKey->Draw(_scale);
	}
}

// �J�E���g�_�E��
void PlayUI::RenderCountDown(const float& countDown)
{
	// �䗦���v�Z
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_countDownEnds -= COUNT_END_SPEED;
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",
			{ (m_windowSize / 2).x * _scale.x,
			((m_windowSize / 2).y + m_countDownEnds * COUNT_MOVE_SPEED) * _scale.y },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			m_windowSize / 2
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

// �I������
void PlayUI::Finalize()
{
}

// �^�C�}�[�̕`��
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

// ���z�̕`��
void PlayUI::RenderSunny(SimpleMath::Vector2 scale)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"Sun",
		m_sunPos,
		{ 1.0f,1.0f,1.0f,1.0f },
		scale,
		SimpleMath::Vector2{ SUN_SIZE / 2 },
		{ 0L,0L,100L,100L }
	);
}

// �y�[�W�̕`��
void PlayUI::RenderHelpPage(SimpleMath::Vector2 scale)
{
	// Page1
	m_system->GetDrawSprite()->DrawTexture(
		L"HowToPlay",
		GetHelpPosition(L"HowToPlay") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);

	// Page2
	m_system->GetDrawSprite()->DrawTexture(
		L"BlockInfo",
		GetHelpPosition(L"BlockInfo") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);

	// Page3
	m_system->GetDrawSprite()->DrawTexture(
		L"GoScenes",
		GetHelpPosition(L"GoScenes") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);
}

// �ʒu���ړ�����
void PlayUI::MovePositions(SimpleMath::Vector2* pos,const SimpleMath::Vector2& end)
{
	(*pos) = UserUtility::Lerp(*pos, end, SimpleMath::Vector2{ PAGE_SPEED });
}

// ���W���Ăяo��
const SimpleMath::Vector2& PlayUI::GetHelpPosition(const wchar_t* key)
{
	std::map<const wchar_t*, HelpPosition>::const_iterator _it = m_helpPoses.find(key);

	return _it->second.nowPos;
}