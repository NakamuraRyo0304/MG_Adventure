/*
 *	@File	UserInterface.cpp
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "UserInterface.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
UserInterface::UserInterface():
	m_systemManager{},				// �V�X�e���}�l�[�W��
	m_aabbCol{},					// �����蔻��
	m_saveTexPos{},					// �摜���W
	m_cameraTexPos{},
	m_penTexPos{},
	m_openTexPos{},
	is_saveFlag{},					// �ۑ��t���O
	is_openFlag{},					// �J���t���O
	is_cameraFlag{ true },			// �J�������[�hON�ŃX�^�[�g
	is_drawFlag{ true }				// �y�����[�h�ŃX�^�[�g		

{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
UserInterface::~UserInterface()
{
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void UserInterface::Initialize(std::shared_ptr<SystemManager> shareSystem,ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �|�C���^�̋��L
	m_systemManager = shareSystem;

	// �摜�̐ݒ�
	m_systemManager->GetDrawSprite()->MakeSpriteBatch(context);
	// �L�[���@�F�@�t�@�C���p�X���@�F�@�f�o�C�X
	m_systemManager->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Pen", L"Resources/Textures/AddBlock.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Open", L"Resources/Textures/OpenFile.dds", device);

	// ���W���
	m_openTexPos = { 80 , 80 };
	m_saveTexPos = { 208  , 80 };
	m_cameraTexPos = { 336 , 80 };
	m_penTexPos = { 464, 80 };
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void UserInterface::Update(Mouse::State& mouseState)
{
	// �t�@�C�����J���A�C�R��
	bool open = false;
	open = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_openTexPos.x,m_openTexPos.y },		    // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },		    // �T�C�Y
		SimpleMath::Vector2{ 100.0f });	    // �T�C�Y
	if (open)
	{
		is_openFlag = true;
	}
	else
	{
		is_openFlag = false;
	}


	// �ۑ��A�C�R�����N���b�N
	bool save = false;
	save = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_saveTexPos.x,m_saveTexPos.y },		    // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },		    // �T�C�Y
		SimpleMath::Vector2{ 100.0f });	    // �T�C�Y
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
	bool camera = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 									// �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f }, 										// �T�C�Y
		SimpleMath::Vector2{ 100.0f });									// �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (camera && m_systemManager->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		is_cameraFlag = !is_cameraFlag;
		m_systemManager->GetCamera()->SetEagleMode(is_cameraFlag);
	}

	// �y��/�����S���A�C�R�����N���b�N
	bool tool = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_penTexPos.x,m_penTexPos.y },	        							 // �摜�̈ʒu
		SimpleMath::Vector2{ 5.0f },									 // �T�C�Y
		SimpleMath::Vector2{ 100.0f });								 // �T�C�Y

	// �`�惂�[�h�؂�ւ�
	if (tool && m_systemManager->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		is_drawFlag = !is_drawFlag;
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void UserInterface::Render()
{
	// �Z�[�u�A�C�R��
	if (is_openFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Open",							// �o�^�L�[
			m_openTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.55f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Open",							// �o�^�L�[
			m_openTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,0.3f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �Z�[�u�A�C�R��
	if (is_saveFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Save",							// �o�^�L�[
			m_saveTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.55f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Save",							// �o�^�L�[
			m_saveTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,0.3f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �J�����A�C�R��
	if (is_cameraFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"CameraMove",						// �o�^�L�[
			m_cameraTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.55,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Camera",							// �o�^�L�[
			m_cameraTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,0.3f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �y���A�C�R��
	if (is_drawFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Pen",								// �o�^�L�[
			m_penTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.55f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Erase",							// �o�^�L�[
			m_penTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
}
