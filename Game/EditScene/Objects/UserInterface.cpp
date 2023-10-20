/*
 *	@File	UserInterface.cpp
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

#include "UserInterface.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
UserInterface::UserInterface(const SimpleMath::Vector2& windowSize)
	: m_system{}					// �V�X�e���}�l�[�W��
	, m_windowSize{windowSize}		// ��ʃT�C�Y
	, m_nowState{}					// �ŐV�̃X�e�[�g
	, m_imageHitter{}				// �����蔻��
	, m_saveTexPos{}				// �Z�[�u�A�C�R���̍��W
	, m_cameraTexPos{}				// �J�����A�C�R���̍��W
	, m_backTexPos{}				// �߂�A�C�R���̍��W
	, m_openTexPos{}				// �t�@�C���A�C�R���̍��W
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

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
UserInterface::~UserInterface()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="shareSystem">�V�X�e���f�[�^</param>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <returns>�Ȃ�</returns>
void UserInterface::Initialize(std::shared_ptr<SystemManager> shareSystem,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �|�C���^�̋��L
	m_system = shareSystem;

	// �摜�̐ݒ�
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �L�[���@�F�@�t�@�C���p�X���@�F�@�f�o�C�X
	// ����A�C�R��
	m_system->GetDrawSprite()->AddTextureData(L"Save",      L"Resources/Textures/SaveFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Open",      L"Resources/Textures/OpenFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Camera",    L"Resources/Textures/Camera.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"CameraMove",L"Resources/Textures/CameraMove.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ToolOn",	L"Resources/Textures/ToolOn.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ToolOff",	L"Resources/Textures/ToolOff.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BackSelect",L"Resources/Textures/BackSelect.dds", device);

	// �w�i�̑�
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar",   L"Resources/Textures/EditToolBar.dds", device);

	// �u���b�N�A�C�R��
	m_system->GetDrawSprite()->AddTextureData(L"Grass",  L"Resources/Textures/BLOCK/GrassIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Cloud",  L"Resources/Textures/BLOCK/CloudIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Coin",   L"Resources/Textures/BLOCK/CoinIcon.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"Reset",  L"Resources/Textures/BLOCK/ResetIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Player", L"Resources/Textures/BLOCK/PlayerIcon.dds",device);
	m_system->GetDrawSprite()->AddTextureData(L"Delete", L"Resources/Textures/BLOCK/DeleteIcon.dds",device);

	// ID���i�[
	m_texName[MAPSTATE::GRASS]   = L"Grass";
	m_texName[MAPSTATE::CLOUD]   = L"Cloud";
	m_texName[MAPSTATE::COIN]    = L"Coin";
	m_texName[MAPSTATE::RESET]   = L"Reset";
	m_texName[MAPSTATE::PLAYER]  = L"Player";
	m_texName[MAPSTATE::NONE]    = L"Delete";

	// �䗦���v�Z
	float _span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_openTexPos	   = {  80 * _span , 80 * _span};
	m_saveTexPos       = { 218 * _span , 80 * _span};
	m_cameraTexPos     = { 356 * _span , 80 * _span};
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

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mouseState">�}�E�X</param>
/// <returns>�Ȃ�</returns>
void UserInterface::Update(Mouse::State& mouseState)
{
	// �c�[���o�[�\���؂�ւ��A�C�R�����N���b�N
	bool _tool = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // �}�E�X�̈ʒu
		{ m_toolButtonTexPos.x,m_toolButtonTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 						 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });						 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (_tool && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_toolFlag = !is_toolFlag;
	}

	// �c�[���o�[�t���O�������Ă�����I���\�ɂ���
	if (!is_toolFlag) return;

	// �{�b�N�X�̃A�C�R��
	ChangeState(mouseState);

	// �Z���N�g�ɖ߂�{�^�����N���b�N
	bool _back = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // �}�E�X�̈ʒu
		{ m_backTexPos.x,m_backTexPos.y },	 				 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 						 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });						 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (_back && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_backFlag = true;
	}

	// �t�@�C�����J���A�C�R��
	is_openFlag = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_openTexPos.x,m_openTexPos.y },		    // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },			    // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y

	// �t�@�C����ۑ�����A�C�R��
	is_saveFlag = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_saveTexPos.x,m_saveTexPos.y },			// �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },				// �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y

	// �J�����A�C�R�����N���b�N
	bool _camera = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y }, // �}�E�X�̈ʒu
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 				 // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				 // �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (_camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_cameraFlag = !is_cameraFlag;
		m_system->GetCamera()->SetEagleMode(is_cameraFlag);
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void UserInterface::Render()
{
	// �摜�̊g�嗦���E�B���h�E�����ƂɌv�Z
	SimpleMath::Vector2 _imageScale = m_windowSize / FULL_SCREEN_SIZE;

	// �c�[���t���O��True�Ȃ�c�[���o�[��\������
	if (is_toolFlag)
	{
		// �c�[���o�[
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolBar",
			SimpleMath::Vector2::Zero,				// ���W
			{ 1.0f,1.0f,1.0f,0.7f },				// �F
			_imageScale,							// �g�嗦
			SimpleMath::Vector2::Zero
		);

		// �t�@�C���A�C�R��
		if (is_openFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// �o�^�L�[
				m_openTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,1.0f },			// �F
				IMAGE_RATE * _imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// �o�^�L�[
				m_openTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.2f },			// �F
				0.5f * _imageScale,					// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}

		// �Z�[�u�A�C�R��
		if (is_saveFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// �o�^�L�[
				m_saveTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,1.0f },			// �F
				IMAGE_RATE * _imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// �o�^�L�[
				m_saveTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.2f },			// �F
				0.5f * _imageScale,					// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}

		// �J�����A�C�R��
		if (is_cameraFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"CameraMove",						// �o�^�L�[
				m_cameraTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,1.0f },			// �F
				IMAGE_RATE * _imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Camera",							// �o�^�L�[
				m_cameraTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.3f },			// �F
				IMAGE_RATE * _imageScale * 0.9f,	// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}

		// �Z���N�g�ɖ߂�{�^��
		m_system->GetDrawSprite()->DrawTexture(
				L"BackSelect",						// �o�^�L�[
				m_backTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,1.0f },			// �F
				IMAGE_RATE * _imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);

		// �u���b�N�̃A�C�R��
		DrawIcon(_imageScale);
	}

	// �c�[���o�[�{�^���\��
	if (is_toolFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOn",								// �o�^�L�[
			m_toolButtonTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },				// �F
			IMAGE_RATE * _imageScale,				// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }			// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOff",								// �o�^�L�[
			m_toolButtonTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },				// �F
			IMAGE_RATE * _imageScale,				// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }			// ���S�ʒu
		);
	}
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void UserInterface::Finalize()
{
}

/// <summary>
/// �A�C�R���̕`��
/// </summary>
/// <param name="imageScale">�g�嗦</param>
/// <returns>�Ȃ�</returns>
void UserInterface::DrawIcon(const SimpleMath::Vector2& imageScale)
{
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		if (is_boxState[i])
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[i],
				SimpleMath::Vector2{ m_imagePos[i].x,m_imagePos[i].y + IMAGE_RATE},
				SimpleMath::Vector4::One,
				IMAGE_RATE * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER }
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[i],
				m_imagePos[i],
				{ 1.0f,1.0f,1.0f,HALF },
				(HALF + m_boxHover[i]) * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER }
			);
		}
	}
}

/// <summary>
/// �X�e�[�^�X�̕ύX
/// </summary>
/// <param name="mouseState">�}�E�X</param>
/// <returns>�Ȃ�</returns>
void UserInterface::ChangeState(DirectX::Mouse::State& mouseState)
{
	// �}�E�X��UI�G���A�ȊO�Ȃ珈�����Ȃ�
	if (mouseState.y > 170.0f * (m_windowSize.y / FULL_SCREEN_SIZE.y)) return;

	// �A�C�R�����Ƃ̏����l
	bool _iconFlags[MAPSTATE::LENGTH] = { false };

	// �e�A�C�R�����N���b�N�������ǂ����𔻒肵�A�t���O�𗧂Ă�
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		_iconFlags[i] = m_imageHitter.IsHitAABB2D(
			{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
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
					 _iconFlags[MAPSTATE::RESET]  ||
					 _iconFlags[MAPSTATE::PLAYER] ||
					 _iconFlags[MAPSTATE::NONE]);

	// �}�E�X���N���b�N���ꂽ�ꍇ�ɁA���݂̃X�e�[�^�X���X�V����
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		// �������Ă��Ȃ���Ώ������Ȃ�
		if (is_anyHitFlag == false) return;

		if (mouseState.leftButton)
		{
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
				case MAPSTATE::RESET:
					m_nowState = MAPSTATE::RESET;		// ���Z�b�g�|�C���g
					break;
				case MAPSTATE::PLAYER:
					m_nowState = MAPSTATE::PLAYER;		// �v���C���[
					break;
				case MAPSTATE::NONE:
					m_nowState = MAPSTATE::NONE;		// ��C
					break;
				default:
					break;
				}
			}
		}
	}
}
