/*
 *	@File	PlayScene.cpp
 *	@Brief	�v���C�V�[��
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

// �}�b�v�T�C�Y(Stage)
#define			BOXSIZE			1.0f


 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_box{},
	m_boxPos{},
	m_map{},
	m_mapData{},
	m_size{},
	m_wantPos{DirectX::SimpleMath::Vector3::Zero},				// �f�o�b�O�p
	m_testPos{DirectX::SimpleMath::Vector3::Zero}				// �f�o�b�O�p
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

	m_camera->SetMoveMode(false);				// �J�������W�ړ�
	m_camera->SetEagleMode(false);				// �J�������_�ړ�

	// �}�b�v�ǂݍ���
	LoadMap(GetStageNum());

	// �{�b�N�X�̏�����
	m_box = DirectX::GeometricPrimitive::CreateBox(
		GetDeviceResources()->GetD3DDeviceContext(),
		DirectX::XMFLOAT3(BOXSIZE, BOXSIZE, BOXSIZE));

	m_sphereColl.SetPushMode(true);

	m_size = 0;
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
	GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	m_camera->Update();

	// ���C�̍X�V
	GetRayCast()->Update(mouseState);

	// �{�b�N�X�̃T�C�Y�l��ύX
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Up))
	{
		m_wantPos.y += BOXSIZE / 2;
		m_size += BOXSIZE;
	}
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Down))
	{
		m_wantPos.y -= BOXSIZE / 2;
		m_size -= BOXSIZE;
	}

	// �{�b�N�X�T�C�Y�ύX�i��X�ύX�j
	m_box = DirectX::GeometricPrimitive::CreateSphere(
		GetDeviceResources()->GetD3DDeviceContext(),BOXSIZE);

	m_test = DirectX::GeometricPrimitive::CreateSphere(
		GetDeviceResources()->GetD3DDeviceContext(),BOXSIZE);

	m_sphereColl.PushSphere(m_wantPos, m_testPos, BOXSIZE / 2, BOXSIZE / 2);

	// �ړ��������ʒu��ݒ�
	if (GetRayCast()->GetClickFlag())
	{
		float save = m_wantPos.y;
		m_wantPos = GetRayCast()->GetWorldMousePosition();
		m_wantPos.y = save;
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();
	// Space�L�[�ŃV�[���؂�ւ�
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
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
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);

	// �J�����p�s��
	DirectX::SimpleMath::Matrix world, view, projection;

	// ���[���h�s��
	world = DirectX::SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = m_camera->GetView();

	// �v���W�F�N�V�����s��
	projection = m_camera->GetProjection(width, height, 45.0f);

	// ���C�̐ݒ�
	GetRayCast()->SetMatrix(view, projection);

	// ���W�ݒ�
	world = DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	// ���̕`��
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_wantPos);
	DirectX::SimpleMath::Matrix world1 = DirectX::SimpleMath::Matrix::CreateTranslation(m_testPos);

	m_box->Draw(world, view, projection, DirectX::Colors::Red);
	m_test->Draw(world1, view, projection, DirectX::Colors::Blue);

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
	auto device  = GetDeviceResources()->GetD3DDevice();
	auto context = GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	CreateUnique(device, context);

	// ��ʃT�C�Y�̊i�[
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);
	
	// �J�����̐ݒ�
	m_camera->GetProjection(width, height, 45.0f);

	// �����̐ݒ�
	GetString()->CreateString(device, context);

	// ���C���y�Ԕ͈͂�ݒ�
	GetRayCast()->SetScreenSize(width, height);
}


//--------------------------------------------------------//
//�f�o�b�O�\��                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetString()->ChangeFontColor(DirectX::Colors::Black);

	// �V�[�����̕\��
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"PlayScene");

	// �������ݒ�
	wchar_t cam[64];

	// �J�����̃|�W�V����
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(m_camera->GetEyePosition().x),
		static_cast<int>(m_camera->GetEyePosition().y),
		static_cast<int>(m_camera->GetEyePosition().z)
	);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,20 }, cam);

	// �}�E�X�̃��[���h���W
	wchar_t mos[64];
	swprintf_s(mos, 64, L"WorldMousePos = %f,%f,%f",
		GetRayCast()->GetWorldMousePosition().x,
		GetRayCast()->GetWorldMousePosition().y,
		GetRayCast()->GetWorldMousePosition().z);

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,40 }, mos);

	// �ۑ����ꂽ���W
	swprintf_s(mos, 64, L"ClickPos = %f,%f,%f",
		m_wantPos.x,
		m_wantPos.y,
		m_wantPos.z);

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,60 }, mos);

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,80 }, num);

	
	// �f�o�C�X�R���e�L�X�g�̎擾�F�O���b�h�̕`��Ɏg�p
	auto context = GetDeviceResources()->GetD3DDeviceContext();
	// �f�o�b�O�O���b�h�̕\��
	GetGridFloor()->Draw(context, GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//�}�b�v�ǂݍ���                                          //
//--------------------------------------------------------//
void PlayScene::LoadMap(int num)
{
	// �t�@�C�����̐錾
	const char* filename = "CleanData";

	// �}�b�v�̕ύX
	switch (num)
	{
	case 1:	
		filename = "Resources/Maps/Stage1.csv";	
		break;
	case 2:	
		filename = "Resources/Maps/Stage2.csv";	
		break;
	case 3:
		filename = "Resources/Maps/Stage3.csv";
		break;
	default:
		filename = "NoStage";
		break;
	}

	// �}�b�v�̓ǂݍ���
	m_map.SetMapData(filename);


	// �}�b�v�̊i�[
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_mapData[y][x] = m_map.GetMapData(x, y);
			m_boxPos[y][x] = { x * BOXSIZE, BOXSIZE / 2, y * BOXSIZE};
		}
	}
}