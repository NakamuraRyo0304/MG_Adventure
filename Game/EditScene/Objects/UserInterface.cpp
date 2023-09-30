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
	m_system->GetDrawSprite()->AddTextureData(L"Grass",     L"Resources/Textures/BLOCK/GrassIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Cloud",     L"Resources/Textures/BLOCK/ClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Coin",      L"Resources/Textures/BLOCK/CoinIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ReCloud",   L"Resources/Textures/BLOCK/ReClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Player",    L"Resources/Textures/BLOCK/PlayerIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Erase",     L"Resources/Textures/BLOCK/DeleteIcon.dds", device);

	// ID���i�[
	m_texName[MapState::GRASS]   = L"Grass";
	m_texName[MapState::CLOUD]   = L"Cloud";
	m_texName[MapState::COIN]    = L"Coin";
	m_texName[MapState::RESET] = L"ReCloud";
	m_texName[MapState::PLAYER]  = L"Player";
	m_texName[MapState::NONE]       = L"Erase";

	// �䗦���v�Z
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_openTexPos	   = {  80 * span , 80 * span};
	m_saveTexPos       = { 218 * span , 80 * span};
	m_cameraTexPos     = { 356 * span , 80 * span};
	m_backTexPos	   = { m_windowSize.x - (244 * span)  ,80 * span};
	m_toolButtonTexPos = { m_windowSize.x - (96 * span)  ,80 * span};
	for (int i = 0; i < MapState::LENGTH; i++)
	{
		m_imagePos[i] = { 528 * span + (192 * span * i) , 80 * span};
		is_boxState[i] = false;
		m_boxHover[i] = 0.0f;
	}

	// �X�e�[�^�X�̏����l�͑��u���b�N
	m_nowState = MapState::GRASS;
	is_boxState[MapState::GRASS] = true;

	// �c�[���o�[��\��
	is_toolFlag = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mouseState">�}�E�X</param>
/// <returns>�Ȃ�</returns>
void UserInterface::Update(Mouse::State& mouseState)
{
	// �c�[���o�[�\���؂�ւ��A�C�R�����N���b�N
	bool tool = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // �}�E�X�̈ʒu
		{ m_toolButtonTexPos.x,m_toolButtonTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 						 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });						 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (tool && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_toolFlag = !is_toolFlag;
	}

	// �c�[���o�[�t���O�������Ă�����I���\�ɂ���
	if (!is_toolFlag) return;

	// �{�b�N�X�̃A�C�R��
	ChangeState(mouseState);

	// �Z���N�g�ɖ߂�{�^�����N���b�N
	bool back = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // �}�E�X�̈ʒu
		{ m_backTexPos.x,m_backTexPos.y },	 				 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 						 // �T�C�Y
		SimpleMath::Vector2{ 80.0f });						 // �T�C�Y

	// �c�[����\������t���O��؂�ւ�
	if (back && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
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
	bool camera = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y }, // �}�E�X�̈ʒu
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 				 // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				 // �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
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
	SimpleMath::Vector2 imageScale = m_windowSize / FULL_SCREEN_SIZE;

	// �c�[���t���O��True�Ȃ�c�[���o�[��\������
	if (is_toolFlag)
	{
		// �c�[���o�[
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolBar",
			SimpleMath::Vector2::Zero,			// ���W
			{ 1.0f,1.0f,1.0f,0.7f },			// �F
			imageScale,							// �g�嗦
			SimpleMath::Vector2::Zero
		);

		// �t�@�C���A�C�R��
		if (is_openFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// �o�^�L�[
				m_openTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,1.0f },			// �F
				IMAGE_RATE * imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// �o�^�L�[
				m_openTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.2f },			// �F
				0.5f * imageScale,					// �g�嗦
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
				IMAGE_RATE * imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// �o�^�L�[
				m_saveTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.2f },			// �F
				0.5f * imageScale,					// �g�嗦
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
				IMAGE_RATE * imageScale,			// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Camera",							// �o�^�L�[
				m_cameraTexPos,						// ���W
				{ 1.0f,1.0f,1.0f,0.3f },			// �F
				IMAGE_RATE * imageScale * 0.9f,		// �g�嗦
				{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
			);
		}

		// �Z���N�g�ɖ߂�{�^��
		m_system->GetDrawSprite()->DrawTexture(
				L"BackSelect",
				m_backTexPos,
				{ 1.0f,1.0f,1.0f,1.0f },
				IMAGE_RATE * imageScale,
				{ IMAGE_CENTER,IMAGE_CENTER }
			);

		// �u���b�N�̃A�C�R��
		DrawIcon(imageScale);
	}

	// �c�[���o�[�{�^���\��
	if (is_toolFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOn",							// �o�^�L�[
			m_toolButtonTexPos,					// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOff",							// �o�^�L�[
			m_toolButtonTexPos,					// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
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
	for (int idx = 0; idx < MapState::LENGTH; ++idx)
	{
		if (is_boxState[idx])
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[idx],							// �o�^�L�[
				m_imagePos[idx],						// ���W
				SimpleMath::Vector4::One,				// �F
				IMAGE_RATE * imageScale,				// �g�嗦
				SimpleMath::Vector2{ IMAGE_CENTER }		// ���S�ʒu
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[idx],							// �o�^�L�[
				m_imagePos[idx],						// ���W
				{ 1.0f,1.0f,1.0f,HALF },				// �F
				(HALF + m_boxHover[idx]) * imageScale,	// �g�嗦
				SimpleMath::Vector2{ IMAGE_CENTER }		// ���S�ʒu
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
	bool iconFlags[MapState::LENGTH] = { false };

	// �e�A�C�R�����N���b�N�������ǂ����𔻒肵�A�t���O�𗧂Ă�
	for (int i = 0; i < MapState::LENGTH; ++i)
	{
		iconFlags[i] = m_imageHitter.IsHitAABB2D(
			{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
			m_imagePos[i],                              // �摜�̈ʒu
			SimpleMath::Vector2{ 5.0f },                // �ŏ��T�C�Y
			SimpleMath::Vector2{ 100.0f });             // �ő�T�C�Y

		// �������Ă�����T�C�Y��傫������(�z�o�[)
		m_boxHover[i] = iconFlags[i] == true ? 0.1f : 0.0f;
	}


	// �}�E�X���N���b�N���ꂽ�ꍇ�ɁA���݂̃X�e�[�^�X���X�V����
	if (mouseState.leftButton)
	{
		for (int i = 0; i < MapState::LENGTH; ++i)
		{
			// �L���t���O���i�[
			is_boxState[i] = iconFlags[i];

			if (iconFlags[i])
			{
				switch (i)
				{
				case MapState::GRASS:
					m_nowState = MapState::GRASS;		// ���u���b�N
					break;
				case MapState::COIN:
					m_nowState = MapState::COIN;			// �R�C��
					break;
				case MapState::CLOUD:
					m_nowState = MapState::CLOUD;		// �_
					break;
				case MapState::RESET:
					m_nowState = MapState::RESET;		// ���Z�b�g�|�C���g
					break;
				case MapState::PLAYER:
					m_nowState = MapState::PLAYER;		// �v���C���[
					break;
				case MapState::NONE:
					m_nowState = MapState::NONE;			// ��C
					break;
				default:
					break;
				}
			}
		}
	}
}
