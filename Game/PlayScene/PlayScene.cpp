/*
 *	@File	PlayScene.cpp
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

 // �}�b�v�T�C�Y(Stage)
#define			COMMON_SIZE			1.0f

// �Œፂ�x
#define			COMMON_LOW			COMMON_SIZE / 2

// �J�����A���O��
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_obj{},						// �}�b�v�̃u���b�N
	m_map{},						// �}�b�v
	m_boxCol{},						// �����̓����蔻��
	m_player{ nullptr },			// �v���C���̃��f��
	m_playerPos{},					// �v���C���̈ʒu
	m_grassBox{ nullptr },			// ���u���b�N�̃��f��
	m_gravity{}
{

}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
PlayScene::~PlayScene()
{
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void PlayScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetMoveMode(false);    	// �J�������W�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(true);		// �J�������_�ړ�

	// �}�b�v�ǂݍ���
	LoadMap(GetStageNum());

	m_playerPos = { 0.0f,2.0f,0.0f };
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
void PlayScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// ���C�̍X�V
	GetSystemManager()->GetRayCast()->Update(mouseState);

	//�@�N���X�����@Player Box�����
	if (keyState.W) m_playerPos.z -= 0.05f;
	if (keyState.S) m_playerPos.z += 0.05f;
	if (keyState.A) m_playerPos.x -= 0.05f;
	if (keyState.D) m_playerPos.x += 0.05f;

	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_playerPos.y += 1.0f;
	}

	m_gravity += 0.01f;

	m_playerPos.y -= m_gravity;

	// �����蔻��̍X�V
	DoBoxCollision();

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Enter))
	{
		GoNextScene(SCENE::RESULT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void PlayScene::Draw()
{
	// ��ʃT�C�Y�̊i�[
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �J�����p�s��
	DirectX::SimpleMath::Matrix world, view, projection;

	// ���[���h�s��
	world = DirectX::SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	projection = GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// ���C�̐ݒ�
	GetSystemManager()->GetRayCast()->SetMatrix(view, projection);

	// �v���C���̕`��
	DirectX::SimpleMath::Matrix playerWorldMat =
		DirectX::SimpleMath::Matrix::CreateTranslation(m_playerPos);
	m_player->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
		*GetSystemManager()->GetCommonStates(), playerWorldMat, view, projection, false);

	// �{�b�N�X�̕`��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_obj[y][x].state % 100; h++)
			{
				m_obj[y][x].position.y = COMMON_LOW + h * COMMON_SIZE; // �Œ���W�{�C�ӂ̍���

				// �{�b�N�X�̈ړ�
				DirectX::SimpleMath::Matrix boxWorldMat =
					DirectX::SimpleMath::Matrix::CreateTranslation(m_obj[y][x].position);

				if (m_obj[y][x].state % 100 == MapLoad::BoxState::None) return;	// �{�b�N�X���Ȃ���Ε`�悵�Ȃ�

				m_grassBox->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
					*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);

			}
		}
	}

	// �f�o�b�O�\��
	DebugLog(view, projection);
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void PlayScene::Finalize()
{
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
void PlayScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);

	// ��ʃT�C�Y�̊i�[
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// �����̐ݒ�
	GetSystemManager()->GetString()->CreateString(device, context);

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);

	// ���f�����쐬����
	m_player = ModelFactory::GetModel(						// �v���C��
		device,
		L"Resources/Models/TestPlayer.cmo"
	);
	m_grassBox = ModelFactory::GetModel(					// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
}

//--------------------------------------------------------//
//�����蔻��̏������܂Ƃ߂��֐�                          //
//--------------------------------------------------------//
void PlayScene::DoBoxCollision()
{
	m_boxCol.SetPushMode(true);
	// �����蔻��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_obj[y][x].state % 100; h++)
			{
				m_obj[y][x].position.y = COMMON_LOW + h * COMMON_SIZE; // �Œ���W�{�C�ӂ̍���

				m_boxCol.PushBox(&m_playerPos, m_obj[y][x].position,   // �v���C�����{�b�N�X
					DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2},	   // �T�C�Y
					DirectX::SimpleMath::Vector3{ COMMON_SIZE }		   // �T�C�Y
				);
			}
		}
	}
	
	// ��ɓ���������d�͂����Z�b�g
	if (m_boxCol.GetHitFace() != Collider::BoxCollider::HIT_FACE::DOWN)
	{
		m_gravity = 0.0f;
	}
}

//--------------------------------------------------------//
//�f�o�b�O�\��                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// �V�[�����̕\��
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"PlayScene");

	// �������ݒ�
	wchar_t cam[64];

	// �J�����̃|�W�V����
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().z)
	);
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,20 }, cam);

	// �}�E�X�̃��[���h���W
	wchar_t mos[64];
	swprintf_s(mos, 64, L"WorldMousePos = %f,%f,%f",
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().x,
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().y,
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,40 }, mos);

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, num);
	
	// �v���C���̍��W
	wchar_t plr[64];
	swprintf_s(plr, 64, L"Player = %f,%f,%f", m_playerPos.x,m_playerPos.y,m_playerPos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, plr);


	// �f�o�C�X�R���e�L�X�g�̎擾�F�O���b�h�̕`��Ɏg�p
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// �f�o�b�O�O���b�h�̕\��
	GetSystemManager()->GetGridFloor()->Draw(context, GetSystemManager()->GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//�}�b�v�ǂݍ���                                          //
//--------------------------------------------------------//
void PlayScene::LoadMap(int num)
{
	// �t�@�C�����̐錾
	std::wstring filePath = L"";

	// �}�b�v�̕ύX
	switch (num)
	{
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	// �}�b�v�̓ǂݍ���
	m_map.LoadMap(filePath);

	// �}�b�v�̊i�[
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// �X�e�[�g���Z�b�g
			m_obj[y][x].state = m_map.GetMapData(x, y);

			// �z��̂��݂�����
			m_obj[y][x].position = DirectX::SimpleMath::Vector3::Zero;
								
			// �{�b�N�X�̈ʒu��������
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),							// �u���b�N�̈ʒu - �I�t�Z�b�g
				COMMON_LOW,						 												// �u���b�N�����ʒu
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)								// �u���b�N�̈ʒu - �I�t�Z�b�g
			};
			
		}
	}
}