/*
 *	@File	UserInterface.cpp
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

#include "UserInterface.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
UserInterface::UserInterface(const DirectX::SimpleMath::Vector2& windowSize):
	m_system{},						// �V�X�e���}�l�[�W��
	m_windowSize{windowSize},		// ��ʃT�C�Y
	m_aabbCol{},					// �����蔻��
	m_saveTexPos{},					// �摜���W
	m_cameraTexPos{},				// �摜�̈ʒu
	m_openTexPos{},					// |
	is_saveFlag{},					// �ۑ��t���O
	is_openFlag{},					// �J���t���O
	is_cameraFlag{ true },			// �J�������[�hON�ŃX�^�[�g
	is_boxState{ false }			// �摜�t���O
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
	m_system->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Open", L"Resources/Textures/OpenFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);

	// �w�i�̑�
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar", L"Resources/Textures/EditToolBar.dds", device);

	// �u���b�N�A�C�R��
	m_system->GetDrawSprite()->AddTextureData(L"Grass", L"Resources/Textures/BLOCK/GrassIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Clowd", L"Resources/Textures/BLOCK/ClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Coin", L"Resources/Textures/BLOCK/CoinIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ReClowd", L"Resources/Textures/BLOCK/ReClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Player", L"Resources/Textures/BLOCK/PlayerIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);

	// ID���i�[
	m_texName[MapState::GrassBox]   = L"Grass";
	m_texName[MapState::ClowdBox]   = L"Clowd";
	m_texName[MapState::CoinBox]    = L"Coin";
	m_texName[MapState::ResetClowd] = L"ReClowd";
	m_texName[MapState::PlayerPos]  = L"Player";
	m_texName[MapState::None]       = L"Erase";

	// �䗦���v�Z
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_openTexPos	= {  80 * span , 80 * span};
	m_saveTexPos    = { 208 * span , 80 * span};
	m_cameraTexPos  = { 336 * span , 80 * span};
	for (int i = 0; i < MapState::LENGTH; i++)
	{
		m_imagePos[i] = { 464 + (192 * span * i) , 80 * span};
		is_boxState[i] = false;
	}

	// �X�e�[�^�X�̏����l�͑��u���b�N
	m_nowState = MapState::GrassBox;
	is_boxState[MapState::GrassBox] = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mouseState">�}�E�X</param>
/// <returns>�Ȃ�</returns>
void UserInterface::Update(Mouse::State& mouseState)
{
	// �{�b�N�X�̃A�C�R��
	ChangeState(mouseState);

	// �t�@�C�����J���A�C�R��
	bool open = false;
	open = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_openTexPos.x,m_openTexPos.y },		    // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },			    // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y
	if (open)
	{
		is_openFlag = true;
	}
	else
	{
		is_openFlag = false;
	}


	// �t�@�C����ۑ�����A�C�R��
	bool save = false;
	save = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_saveTexPos.x,m_saveTexPos.y },			// �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },				// �T�C�Y
		SimpleMath::Vector2{ 100.0f });				// �T�C�Y
	// �����Ă��ăN���b�N����������
	if (save)
	{
		is_saveFlag = true;
	}
	else
	{
		is_saveFlag = false;
	}

	// �J�����A�C�R�����N���b�N
	bool camera = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y }, // �}�E�X�̈ʒu
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 				 // �T�C�Y
		SimpleMath::Vector2{ 100.0f });				 // �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
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
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

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
			{ 1.0f,1.0f,1.0f,1.0f},				// �F
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
			IMAGE_RATE* imageScale,				// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Camera",							// �o�^�L�[
			m_cameraTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,0.3f },			// �F
			IMAGE_RATE* imageScale,				// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �u���b�N�̃A�C�R��
	DrawIcon(imageScale);
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
void UserInterface::DrawIcon(const float& imageScale)
{
	// ���u���b�N
	if (is_boxState[MapState::GrassBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Grass",							// �o�^�L�[
			m_imagePos[MapState::GrassBox],		// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Grass",							// �o�^�L�[
			m_imagePos[MapState::GrassBox],		// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �_�u���b�N
	if (is_boxState[MapState::ClowdBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Clowd",							// �o�^�L�[
			m_imagePos[MapState::ClowdBox],		// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Clowd",							// �o�^�L�[
			m_imagePos[MapState::ClowdBox],		// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �R�C���u���b�N
	if (is_boxState[MapState::CoinBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Coin",							// �o�^�L�[
			m_imagePos[MapState::CoinBox],		// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Coin",							// �o�^�L�[
			m_imagePos[MapState::CoinBox],		// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �_���Z�b�g�u���b�N
	if (is_boxState[MapState::ResetClowd])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ReClowd",							// �o�^�L�[
			m_imagePos[MapState::ResetClowd],	// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ReClowd",							// �o�^�L�[
			m_imagePos[MapState::ResetClowd],	// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �v���C���[�u���b�N
	if (is_boxState[MapState::PlayerPos])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Player",							// �o�^�L�[
			m_imagePos[MapState::PlayerPos],	// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Player",							// �o�^�L�[
			m_imagePos[MapState::PlayerPos],	// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �����S��
	if (is_boxState[MapState::None])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Erase",							// �o�^�L�[
			m_imagePos[MapState::None],			// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Erase",							// �o�^�L�[
			m_imagePos[MapState::None],			// ���W
			{ 1.0f,1.0f,1.0f,0.5f },			// �F
			IMAGE_RATE * imageScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
}

/// <summary>
/// �X�e�[�^�X�̕ύX
/// </summary>
/// <param name="mouseState">�}�E�X</param>
/// <returns>�Ȃ�</returns>
void UserInterface::ChangeState(DirectX::Mouse::State& mouseState)
{
	// �A�C�R�����Ƃ̏����l
	bool iconFlags[MapState::LENGTH] = { false };

	// �e�A�C�R�����N���b�N�������ǂ����𔻒肵�A�t���O�𗧂Ă�
	for (int i = 0; i < MapState::LENGTH; ++i)
	{
		iconFlags[i] = m_aabbCol.HitAABB_2D(
			{ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
			m_imagePos[i],                              // �摜�̈ʒu
			SimpleMath::Vector2{ 5.0f },                // �T�C�Y
			SimpleMath::Vector2{ 100.0f });             // �T�C�Y
	}

	// �}�E�X���N���b�N���ꂽ�ꍇ�ɁA���݂̃X�e�[�^�X���X�V����
	if (mouseState.leftButton)
	{
		for (int i = 0; i < MapState::LENGTH; ++i)
		{
			is_boxState[i] = iconFlags[i];
			if (iconFlags[i])
			{
				switch (i)
				{
				case MapState::GrassBox:
					m_nowState = MapState::GrassBox;
					break;
				case MapState::CoinBox:
					m_nowState = MapState::CoinBox;
					break;
				case MapState::ClowdBox:
					m_nowState = MapState::ClowdBox;
					break;
				case MapState::ResetClowd:
					m_nowState = MapState::ResetClowd;
					break;
				case MapState::PlayerPos:
					m_nowState = MapState::PlayerPos;
					break;
				case MapState::None:
					m_nowState = MapState::None;
					break;
				default:
					break;
				}
			}
		}
	}
}
