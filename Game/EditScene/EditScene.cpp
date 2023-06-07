/*
 *	@File	EditScene.cpp
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "EditScene.h"

// �}�b�v�T�C�Y(Stage)
#define			COMMON_SIZE			1.0f

// �Œፂ�x
#define			COMMON_LOW			COMMON_SIZE / 2

// �J�����A���O��
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_sphere{},						// ��
	m_spherePos{},
	m_box{},						// ��
	m_obj{},
	m_map{},						// �}�b�v
	m_boxCol{},						// �����̓����蔻��
	m_grassBox{ nullptr },			// ���f��
	m_grassBoxDark{ nullptr }
{

}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
EditScene::~EditScene()
{
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void EditScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetMoveMode(true);	    	// �J�������W�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(true);		// �J�������_�ړ�

	// �X�t�B�A�̏�����(�e�X�g)
	m_sphere = DirectX::GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		COMMON_SIZE / 2
	);

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
void EditScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
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
	
	if (keyState.Z) m_spherePos.y += 0.1f;
	if (keyState.X) m_spherePos.y -= 0.1f;

	// �����蔻��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_boxCol.PushBox(&m_spherePos, m_obj[y][x].position,							// �X�t�B�A���{�b�N�X
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2},								// �T�C�Y
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }									// �T�C�Y
			);
			
			// �����Ă�����True�ɂ���
			m_obj[y][x].hitFlag = m_boxCol.GetHitBoxFlag();

			m_aabbCol.HitAABB(m_spherePos, m_obj[y][x].position,	// �X�t�B�A���{�b�N�X
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2},	    // �T�C�Y
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }			// �T�C�Y
			);

			// �����Ă���Ƃ��ɉE�N���b�N�ŕϓ�
			if (m_obj[y][x].hitFlag && 
				GetSystemManager()->GetMouseTrack()->rightButton && !keyState.LeftShift)
			{
				m_obj[y][x].state += 1;
				if (m_obj[y][x].state % 100 > 15)
				{
					m_obj[y][x].state = MapLoad::BoxState::GrassBox;
				}
			}
			// �����Ă���Ƃ��ɉE�N���b�N�ŕϓ�
			if (m_obj[y][x].hitFlag &&
				GetSystemManager()->GetMouseTrack()->rightButton && keyState.LeftShift)
			{
				m_obj[y][x].state -= 1;
				if (m_obj[y][x].state % 100 < 1)
				{
					m_obj[y][x].state = MapLoad::BoxState::GrassBox + 15;
				}
			}
			// �|�W�V�����̕ύX
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),		// �u���b�N�̈ʒu - �I�t�Z�b�g
				COMMON_LOW + m_obj[y][x].state % 100 * COMMON_SIZE,			// �Œፂ�x + ���x * �T�C�Y
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// �u���b�N�̈ʒu - �I�t�Z�b�g
			};
		}
	}

	// �J�����ړ����[�h�؂�ւ��@�f�o�b�O�p
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::M))
	{
		auto flag = GetSystemManager()->GetCamera();
		flag->SetMoveMode(!flag->GetMoveMode());
		flag->SetEagleMode(!flag->GetEagleMode());
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();
	
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Enter))
	{
		// ���e���L�^
		for (int y = 0; y < m_map.MAP_RAW; y++)
		{
			for (int x = 0; x < m_map.MAP_COLUMN; x++)
			{
				m_map.SetMapData(m_obj[y][x].state, x, y);
			}
		}
		// �t�@�C�������o��
		m_map.WriteMap();
	}

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::RESULT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void EditScene::Draw()
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

	// �{�b�N�X�̕`��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_obj[y][x].state % 100; h++)
			{
				m_obj[y][x].position.y = h * COMMON_SIZE;

				// �{�b�N�X�̈ړ�
				DirectX::SimpleMath::Matrix boxWorldMat =
					DirectX::SimpleMath::Matrix::CreateTranslation(m_obj[y][x].position);

				if (m_obj[y][x].state % 100 == 0) return;	// �{�b�N�X���Ȃ���Ε`�悵�Ȃ�
				
				// �`�揈��
				if (m_obj[y][x].hitFlag && GetSystemManager()->GetRayCast()->GetClickFlag())
				{
					m_grassBoxDark->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, true);
				}
				else if (m_obj[y][x].hitFlag)
				{
					m_grassBoxDark->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
				}
				else
				{
					m_grassBox->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
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
void EditScene::Finalize()
{
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
void EditScene::CreateWindowDependentResources()
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
	m_grassBox = ModelFactory::GetModel(					// ���u���b�N-�ʏ펞
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_grassBoxDark = ModelFactory::GetModel(				// ���u���b�N-�I����
		device,
		L"Resources/Models/GrassBlock_Dark.cmo"
	);
}


//--------------------------------------------------------//
//�f�o�b�O�\��                                            //
//--------------------------------------------------------//
void EditScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// �V�[�����̕\��
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"EditScene");

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
void EditScene::LoadMap(int num)
{
	// ��̃t�@�C���p�X��p�ӂ���
	std::wstring filePath = L"";

	// �}�b�v�̓ǂݍ���
	m_map.LoadMap(filePath);

	// �}�b�v�̊i�[
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// �ǂݍ��񂾃f�[�^���i�[
			m_obj[y][x].state = m_map.GetMapData(x, y);
			
			// �z��̂��݂�����
			m_obj[y][x].position = DirectX::SimpleMath::Vector3::Zero;

			// �{�b�N�X�̈ʒu��������
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),		// �u���b�N�̈ʒu - �I�t�Z�b�g
				COMMON_LOW,													// �u���b�N�̍Œፂ�x
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// �u���b�N�̈ʒu - �I�t�Z�b�g
			};
		}
	}
}