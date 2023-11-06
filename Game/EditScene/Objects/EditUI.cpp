/*
 *	@File	EditUI.cpp
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemDatas/Input.h"
#include "../../../Libraries/SystemManager/SystemManager.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

#include "EditUI.h"

// �R���X�g���N�^
EditUI::EditUI()
	: m_system{}					// �V�X�e���}�l�[�W��
	, m_windowSize{}				// ��ʃT�C�Y
	, m_nowState{}					// �ŐV�̃X�e�[�g
	, m_imageHitter{}				// �����蔻��
	, m_toolTexPos{}				// �c�[���A�C�R���̍��W
	, m_backTexPos{}				// �߂�A�C�R���̍��W
	, m_boxHover{}					// �z�o�[���̃T�C�Y
	, is_anyHitFlag{ false }		// �z�o�[�t���O
	, is_boxState{ false }			// �摜�t���O
	, is_saveFlag{ false }			// �ۑ��t���O
	, is_openFlag{ false }			// �J���t���O
	, is_cameraFlag{ true }			// �J�������[�hON�ŃX�^�[�g
	, is_backFlag{ false }			// �Z���N�g�ɖ߂�t���O
	, is_toolFlag{ true }			// �c�[���o�[��\������t���O
{
}

// �f�X�g���N�^
EditUI::~EditUI()
{
	Finalize();
}

// �쐬�֐�
void EditUI::Create(const std::shared_ptr<SystemManager>& system, const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// �L�[���@�F�@�t�@�C���p�X��
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar",		// �c�[���o�[
		L"Resources/Textures/EDITS/ToolBar.dds");

	m_system->GetDrawSprite()->AddTextureData(L"DoBar",			// ����K�C�h
		L"Resources/Textures/EDITS/UnderInfo.dds");

	m_system->GetDrawSprite()->AddTextureData(L"IconPack",		// �A�C�R���p�b�N
		L"Resources/Textures/EDITS/IconPack.dds");

	m_system->GetDrawSprite()->AddTextureData(L"ModePack",		// ���[�h�p�b�N
		L"Resources/Textures/EDITS/ToolModes.dds");

	m_system->GetDrawSprite()->AddTextureData(L"SavePack",		// �Z�[�u�p�b�N
		L"Resources/Textures/EDITS/SavePack.dds");

	// �؂���ʒu���w��
	m_texRect[MAPSTATE::NONE]    = { _0,_1,_1,_2 };		// �u���b�N�A�C�R��
	m_texRect[MAPSTATE::GRASS]   = { _0,_0,_1,_1 };
	m_texRect[MAPSTATE::CLOUD]   = { _1,_0,_2,_1 };
	m_texRect[MAPSTATE::COIN]    = { _2,_0,_3,_1 };
	m_texRect[MAPSTATE::GRAVITY] = { _2,_1,_3,_2 };
	m_texRect[MAPSTATE::PLAYER]  = { _1,_1,_2,_2 };
	m_modeRect[0] = { _0,_0,_1,_1 };					// �\���ؑցE�߂�A�C�R��
	m_modeRect[1] = { _1,_0,_2,_1 };
	m_modeRect[2] = { _2,_0,_3,_1 };
	m_saveRect[0] = { _0,_0,_1,_1 };					// �J�����ƃt�@�C���A�C�R��
	m_saveRect[1] = { _1,_0,_2,_1 };
	m_saveRect[2] = { _2,_0,_3,_1 };
	m_saveRect[3] = { _3,_0,_4,_1 };

	// �䗦���v�Z
	float _span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_toolTexPos[0] = {80 * _span , 80 * _span};
	m_toolTexPos[1] = {218 * _span , 80 * _span};
	m_toolTexPos[2] = {356 * _span , 80 * _span};
	m_backTexPos	   = { m_windowSize.x - (244 * _span)  ,80 * _span};
	m_toolButtonTexPos = { m_windowSize.x - (96 * _span)  ,80 * _span};

	// �ݒu�u���b�N�A�C�R��
	for (int i = 0; i < MAPSTATE::LENGTH; i++)
	{
		m_imagePos[i] = { 545 * _span + (192 * _span * i) , 80 * _span};
		is_boxState[i] = false;
		m_boxHover[i] = 0.0f;
	}

	// �X�e�[�^�X�̏����l�͑��u���b�N
	m_nowState = MAPSTATE::GRASS;
	is_boxState[MAPSTATE::GRASS] = true;

	// �c�[���o�[��\��
	is_toolFlag = true;

	// �z�o�[�t���O�����Z�b�g
	is_anyHitFlag = false;
}

// �X�V����
void EditUI::Update()
{
	auto& _input = Input::GetInstance();
	auto _mouse = Mouse::Get().GetState();

	// �c�[���o�[�\���؂�ւ��A�C�R�����N���b�N
	bool _tool = m_imageHitter.IsHitAABB2D(
		{ (float)_mouse.x,(float)_mouse.y },				 // �}�E�X�̈ʒu
		{ m_toolButtonTexPos.x,m_toolButtonTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 						 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });						 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (_tool && _input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_toolFlag = !is_toolFlag;
	}

	// �c�[���o�[�t���O�������Ă�����I���\�ɂ���
	if (!is_toolFlag) return;

	// �{�b�N�X�̃A�C�R��
	ChangeState();

	// �Z���N�g�ɖ߂�{�^�����N���b�N
	bool _back = m_imageHitter.IsHitAABB2D(
		{ (float)_mouse.x,(float)_mouse.y },		 // �}�E�X�̈ʒu
		{ m_backTexPos.x,m_backTexPos.y },	 		 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 				 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });				 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (_back && _input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_backFlag = true;
	}

	// �t�@�C�����J���A�C�R��
	is_openFlag = m_imageHitter.IsHitAABB2D(
		{ (float)_mouse.x,(float)_mouse.y },// �}�E�X�̈ʒu
		m_toolTexPos[0],							// �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },			    // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y

	// �t�@�C����ۑ�����A�C�R��
	is_saveFlag = m_imageHitter.IsHitAABB2D(
		{ (float)_mouse.x,(float)_mouse.y },// �}�E�X�̈ʒu
		m_toolTexPos[1],							// �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },				// �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y

	// �J�����A�C�R�����N���b�N
	bool _camera = m_imageHitter.IsHitAABB2D(
		{ (float)_mouse.x,(float)_mouse.y }, // �}�E�X�̈ʒu
		m_toolTexPos[2],						 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 				 // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				 // �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (_camera && _input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_cameraFlag = !is_cameraFlag;
		m_system->GetCamera()->SetEagleMode(is_cameraFlag);
	}
}

// �`�揈��
void EditUI::Render()
{
	// �摜�̊g�嗦���E�B���h�E�����ƂɌv�Z
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;

	// �c�[���t���O��True�Ȃ�c�[���o�[��\������
	if (is_toolFlag)
	{
		// �c�[���o�[ �L�[�F���W�F�F�F�g�嗦�F���S�ʒu�F�摜�T�C�Y
		m_system->GetDrawSprite()->DrawTexture(L"ToolBar", SimpleMath::Vector2::Zero,
			{ 1.0f,1.0f,1.0f,0.7f }, _rate, SimpleMath::Vector2::Zero, RECT_U(0L, 0L, 1920L, 170L));

		// �t�@�C���A�C�R��
		if (is_openFlag)
		{
			// �A�C�R���@�L�[�F���W�F�F�F�g�嗦�F���S�ʒu�F�摜�T�C�Y
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[0],
				SimpleMath::Vector4::One, IMAGE_RATE * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[0]);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[0],
				{ 1.0f,1.0f,1.0f,0.2f }, 0.5f * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[0]);
		}

		// �Z�[�u�A�C�R��
		if (is_saveFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[1],
				SimpleMath::Vector4::One, IMAGE_RATE * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[1]);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[1],
				{ 1.0f,1.0f,1.0f,0.2f }, 0.5f * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[1]);
		}

		// �J�����A�C�R��
		if (is_cameraFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[2],
				SimpleMath::Vector4::One, IMAGE_RATE * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[2]);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(L"SavePack", m_toolTexPos[2],
				{ 1.0f,1.0f,1.0f,0.2f }, 0.5f * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_saveRect[3]);
		}

		// �Z���N�g�ɖ߂�{�^��
		m_system->GetDrawSprite()->DrawTexture(L"ModePack", m_backTexPos,
			SimpleMath::Vector4::One, IMAGE_RATE * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_modeRect[2]);

		// �u���b�N�̃A�C�R��
		DrawIcon(_rate);
	}

	// �c�[���o�[�{�^���\��
	m_system->GetDrawSprite()->DrawTexture(L"ModePack", m_toolButtonTexPos,
		SimpleMath::Vector4::One, IMAGE_RATE * _rate, { FONT_HEIGHT,FONT_HEIGHT }, m_modeRect[is_toolFlag ? 0 : 1]);

	// ��������\��
	SimpleMath::Vector2 _doRate = _rate * 0.5f;
	m_system->GetDrawSprite()->DrawTexture(L"DoBar",
		SimpleMath::Vector2{ 0,m_windowSize.y - static_cast<float>(DO_BAR_RECT.bottom) * _doRate.y }, // �摜�̃T�C�Y����ɏグ��
		SimpleMath::Vector4::One, _doRate, SimpleMath::Vector2::Zero, DO_BAR_RECT);
}

// �I������
void EditUI::Finalize()
{
}

// �A�C�R���`��
void EditUI::DrawIcon(const SimpleMath::Vector2& imageScale)
{
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		if (is_boxState[i])
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"IconPack",
				SimpleMath::Vector2{ m_imagePos[i].x,m_imagePos[i].y + IMAGE_RATE },
				SimpleMath::Vector4::One,
				IMAGE_RATE * imageScale,
				SimpleMath::Vector2{ FONT_HEIGHT },
				m_texRect[i]
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"IconPack",
				m_imagePos[i],
				{ 1.0f,1.0f,1.0f,HALF },
				(HALF + m_boxHover[i]) * imageScale,
				SimpleMath::Vector2{ FONT_HEIGHT },
				m_texRect[i]
			);
		}
	}
}

// �X�e�[�^�X�ύX
void EditUI::ChangeState()
{
	auto _mouse = Mouse::Get().GetState();

	// �}�E�X��UI�G���A�ȊO�Ȃ珈�����Ȃ�
	if (_mouse.y > BAR_HEIGHT * (m_windowSize.y / FULL_SCREEN_SIZE.y)) return;

	// �A�C�R�����Ƃ̏����l
	bool _iconFlags[MAPSTATE::LENGTH] = { false };

	// �e�A�C�R�����N���b�N�������ǂ����𔻒肵�A�t���O�𗧂Ă�
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		_iconFlags[i] = m_imageHitter.IsHitAABB2D(
			{ (float)_mouse.x,(float)_mouse.y },		// �}�E�X�̈ʒu
			m_imagePos[i],                              // �摜�̈ʒu
			SimpleMath::Vector2{ 5.0f },                // �ŏ��T�C�Y
			SimpleMath::Vector2{ 100.0f });             // �ő�T�C�Y

		// �������Ă�����T�C�Y��傫������(�z�o�[)
		m_boxHover[i] = _iconFlags[i] == true ? 0.1f : 0.0f;
	}

	// �ǂꂩ��������Ă�����
	is_anyHitFlag = (_iconFlags[MAPSTATE::GRASS]  ||
					 _iconFlags[MAPSTATE::COIN]   ||
					 _iconFlags[MAPSTATE::CLOUD]  ||
					 _iconFlags[MAPSTATE::GRAVITY]||
					 _iconFlags[MAPSTATE::PLAYER] ||
					 _iconFlags[MAPSTATE::NONE]);

	// �}�E�X���N���b�N���ꂽ�ꍇ�ɁA���݂̃X�e�[�^�X���X�V����
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		// �������Ă��Ȃ���Ώ������Ȃ�
		if (not is_anyHitFlag) return;

		if (not _mouse.leftButton) return;

		// �N���b�N��
		auto& _sound = m_system->GetSoundManager();
		_sound->SetVolume(_sound->GetVolume(XACT_WAVEBANK_SKBX_SE_ICONTAP) / 2, XACT_WAVEBANK_SKBX_SE_ICONTAP);
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);

		// �L���t���O���i�[
		is_boxState[i] = _iconFlags[i];

		if (_iconFlags[i])
		{
			switch (i)
			{
			case MAPSTATE::GRASS:
				m_nowState = MAPSTATE::GRASS;		// ���u���b�N
				break;
			case MAPSTATE::COIN:
				m_nowState = MAPSTATE::COIN;		// �R�C��
				break;
			case MAPSTATE::CLOUD:
				m_nowState = MAPSTATE::CLOUD;		// �_
				break;
			case MAPSTATE::GRAVITY:
				m_nowState = MAPSTATE::GRAVITY;		// ���Z�b�g�|�C���g
				break;
			case MAPSTATE::PLAYER:
				m_nowState = MAPSTATE::PLAYER;		// �v���C���[
				break;
			case MAPSTATE::NONE:
				m_nowState = MAPSTATE::NONE;		// �Ȃ�
				break;
			default:
				break;
			}
		}
	}
}
