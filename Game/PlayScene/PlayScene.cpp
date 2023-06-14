/*
 *	@File	PlayScene.cpp
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_mapObj{},						// �}�b�v�̃u���b�N
	m_map{},						// �}�b�v
	is_boxCol{},					// �����̓����蔻��
	m_playerModel{ nullptr },		// �v���C���̃��f��
	m_playerPos{},					// �v���C���̈ʒu
	m_grassModel{ nullptr },		// ���u���b�N�̃��f��
	m_coinModel{ nullptr },			// �R�C���u���b�N�̃��f��
	m_clowdModel{ nullptr },		// �_�u���b�N�̃��f��
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

	m_playerPos = { 0.0f,5.0f,0.0f };
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

	m_gravity += 0.01f;

	m_playerPos.y -= m_gravity;

	// �����蔻��̍X�V
	DoBoxCollision();


	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::PLAY);
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Enter�L�[�ŃV�[���؂�ւ�
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

	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

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
	m_coinModel->Draw(context, states, playerWorldMat, view, projection, false);

	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		DirectX::SimpleMath::Matrix boxMat =
			DirectX::SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
	}

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
	m_playerModel = ModelFactory::GetModel(						// �v���C��
		device,
		L"Resources/Models/TestPlayer.cmo"
	);
	m_grassModel = ModelFactory::GetModel(						// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetModel(						// �R�C���u���b�N
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetModel(						// �_�u���b�N
		device,
		L"Resources/Models/Clowd.cmo"
	);
}

//--------------------------------------------------------//
//�����蔻��̏������܂Ƃ߂��֐�                          //
//--------------------------------------------------------//
void PlayScene::DoBoxCollision()
{
	// �}�b�v�Ƃ̓����蔻��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �����蔻������
		is_boxCol.PushBox(m_playerPos, m_mapObj[i].position,
			DirectX::SimpleMath::Vector3{ COMMON_SIZE },
			DirectX::SimpleMath::Vector3{ COMMON_SIZE });

		if (is_boxCol.GetHitBoxFlag()&& m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			switch (is_boxCol.GetHitFace())
			{
			case Collider::BoxCollider::HIT_FACE::UP:
				m_gravity = 0.0f;
				m_playerPos.y = m_mapObj[i].position.y + COMMON_SIZE;
				break;
			default:
				break;
			}
		}
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
	m_mapObj = m_map.GetMapData();

	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_mapObj[i].position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}