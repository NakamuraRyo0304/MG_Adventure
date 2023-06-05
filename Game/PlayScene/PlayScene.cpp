/*
 *	@File	PlayScene.cpp
 *	@Brief	�v���C�V�[��
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

// �}�b�v�T�C�Y(Stage)
#define			COMMON_SIZE			1.0f

// �J�����A���O��
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_sphere{},			// ��
	m_spherePos{},
	m_box{},			// ��
	m_boxesPos{},
	m_map{},			// �}�b�v
	m_mapData{},
	m_boxCol{},			// �����̓����蔻��
	is_boxesHitFlag{},
	m_boxModel{nullptr}	// ���f��
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

	GetSystemManager()->GetCamera()->SetMoveMode(true);	    	// �J�������W�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);		// �J�������_�ړ�

	// �X�t�B�A�̏�����(�e�X�g)
	m_sphere = DirectX::GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), COMMON_SIZE / 2);

	// �{�b�N�X�̏�����(�e�X�g)
	m_box = DirectX::GeometricPrimitive::CreateBox(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		DirectX::XMFLOAT3(COMMON_SIZE, COMMON_SIZE, COMMON_SIZE)
	);

	// �}�b�v�ǂݍ���
	LoadMap(GetStageNum());

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

	// �ړ��������ʒu��ݒ�
	m_spherePos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_spherePos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;
	
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Z)) m_spherePos.y+=1;
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::X)) m_spherePos.y-=1;

	// �����蔻��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_boxCol.PushBox(&m_spherePos, m_boxesPos[y][x],		// �X�t�B�A���{�b�N�X
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2 },	// �T�C�Y
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }			// �T�C�Y
			);
			is_boxesHitFlag[y][x] = m_boxCol.GetHitBoxFlag();

			//// �����Ă���Ƃ��ɉE�N���b�N�ŕϓ�
			//if (is_boxesHitFlag[y][x] && mouseState.rightButton)
			//{
			//	m_boxesPos[y][x].y += COMMON_SIZE / 2;
			//	if (m_boxesPos[y][x].y > COMMON_SIZE / 2 * 10)
			//	{
			//		m_boxesPos[y][x].y = COMMON_SIZE / 2;
			//	}
			//}
		}
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
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
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
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

	// ���̕`��
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_spherePos);

	m_sphere->Draw(world, view, projection, DirectX::Colors::Red);

	// ���W�ݒ�
	world = DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	

	// �{�b�N�X�̕`��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// �{�b�N�X�̈ړ�
			DirectX::SimpleMath::Matrix boxWorldMat = DirectX::SimpleMath::Matrix::CreateTranslation(m_boxesPos[y][x]);

			if (m_mapData[y][x] != 0)
			{
				if (is_boxesHitFlag[y][x] && GetSystemManager()->GetRayCast()->GetClickFlag())
				{
					m_box->Draw(boxWorldMat, view, projection, DirectX::Colors::Red);
				}
				else if (is_boxesHitFlag[y][x])
				{
					m_box->Draw(boxWorldMat, view, projection, DirectX::Colors::LightPink);
				}
				else
				{
					m_boxModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
				}
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
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);

	// ��ʃT�C�Y�̊i�[
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);
	
	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// �����̐ݒ�
	GetSystemManager()->GetString()->CreateString(device, context);

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);
	
	// ���f�����쐬����
	m_boxModel = ModelFactory::GetModel(
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
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

	// �ۑ����ꂽ���W
	swprintf_s(mos, 64, L"ClickPos = %f,%f,%f",
		m_spherePos.x,
		m_spherePos.y,
		m_spherePos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, num);

	
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
	const wchar_t* filename = L"CleanData";

	// �}�b�v�̕ύX
	switch (num)
	{
	case 1:	
		filename = L"Resources/Maps/Stage1.csv";
		break;
	case 2:	
		filename = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filename = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filename = L"NoStage";
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
			
			m_boxesPos[y][x] = { 0,0,0 };

			m_boxesPos[y][x] =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE) ,	// �u���b�N�̈ʒu - �I�t�Z�b�g
				COMMON_SIZE / 2,											// ���x
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// �u���b�N�̈ʒu - �I�t�Z�b�g
			};
		}
	}
}