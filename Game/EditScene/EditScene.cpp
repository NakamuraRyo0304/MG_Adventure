/*
 *	@File	EditScene.cpp
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// UI
#include "Objects/UserInterface.h"

#include "EditScene.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_userInterface{},				// UI
	m_sphere{},						// ��
	m_cursorPos{0.0f,0.0f,0.0f},
	m_mapObj{0},					// �i�[�z��
	m_nowState{},					// ���݂̃u���b�N�̎��
	m_timer{},
	m_map{},						// �}�b�v
	is_boxCol{},					// �����̓����蔻��
	m_grassModel{ nullptr },		// ���f��
	m_noneModel{ nullptr },	
	m_coinModel{ nullptr },
	m_clowdModel{ nullptr },
	m_switchModel{ nullptr },
	m_sharedSystem{}
	
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
EditScene::~EditScene()
{
	m_map.~MapLoad();
	Finalize();
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void EditScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ���L���ɂ���
	GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());

	// �X�t�B�A�̏�����(�e�X�g)
	m_sphere = GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		COMMON_SIZE / 2
	);

	// �}�b�v�ǂݍ���//����ǂݍ���
	LoadMap(L"Resources/Maps/Stage1.csv");

	// �����l�͑��u���b�N
	m_nowState = MapState::GrassBox;
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
void EditScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();
	
	// ���C�̍X�V
	GetSystemManager()->GetRayCast()->Update(mouseState);

	// UI�̏���
	m_userInterface->Update(mouseState);

	// �Z�[�u�t���O����������t�@�C����ۑ�
	if (m_userInterface->GetSaveFlag() && 
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		SaveFile();
	}

	// �I�[�v���t���O����������t�@�C�����J��
	if (m_userInterface->GetOpenFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		m_map.LoadMap(L"");				// �t�@�C���V�K�쐬
		m_mapObj = m_map.GetMapData();	// �ǂݍ���
		OffsetPosition_Read(&m_mapObj);	// ���W�␳
	}

	// �V�t�g�������Ă���Ԃ̓J�������[�h��True�ɂ���
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::LeftShift))
	{
		// �C���^�[�t�F�[�X�ŃJ�����̃t���O���擾
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}

	// �X�e�[�^�X�ύX
	if (GetSystemManager()->GetMouseTrack()->rightButton == Mouse::ButtonStateTracker::RELEASED)
	{
		switch (m_nowState)
		{
		case MapState::GrassBox:				// �����R�C��
			ChangeState(MapState::CoinBox);
			break;
		case MapState::CoinBox:					// �R�C�����X�C�b�`
			ChangeState(MapState::SwitchBox);
			break;
		case MapState::SwitchBox:				// �X�C�b�`�������S��
			ChangeState(MapState::None);
			m_userInterface->SetDrawFlag(false);
			break;
		case MapState::None:					// �����S������
			ChangeState(MapState::GrassBox);
			m_userInterface->SetDrawFlag(true);
			break;
		default:
			break;
		}
	}
	// �����S�����[�h�Ȃ�A�I���u���b�N�������S���ɂ���
	if (!m_userInterface->GetDrawFlag())
	{
		ChangeState(MapState::None);
	}


	// �J�������[�h����Ȃ���ΕҏW�ł���
	if (!m_userInterface->GetCameraFlag())
	{
		EditMap();
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void EditScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix world, view, projection;

	// ���[���h�s��
	world = SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	projection = GetSystemManager()->GetCamera()->GetProjection();

	// ���C�̐ݒ�
	GetSystemManager()->GetRayCast()->SetMatrix(view, projection);

	// �s��v�Z
	SimpleMath::Matrix scale = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix rotateX = SimpleMath::Matrix::CreateRotationX(m_timer);
	SimpleMath::Matrix rotateY = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix rotateZ = SimpleMath::Matrix::CreateRotationZ(m_timer);
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix boxMat = 
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapState::CoinBox)
		{
			m_coinModel->Draw(context, states, rotateY * boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapState::SwitchBox)
		{
			m_switchModel->Draw(context, states, boxMat, view, projection);
		}
	}


	// �T�C�Y�@�~�@��]�@�~�@�ړ�
	world *= scale * rotateX * rotateY * rotateZ * trans;

	// �I�����Ă���I�u�W�F��`�� 
	if (m_nowState == MapState::GrassBox)
	{
		m_grassModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::CoinBox)
	{
		m_coinModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::SwitchBox)
	{
		m_switchModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::None)
	{
		m_noneModel->Draw(context, states, world, view, projection);
	}



	// �摜�̕`��
	m_userInterface->Render();

	// �f�o�b�O�\��
	DebugLog(view, projection);
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void EditScene::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// ���f���̔j��
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_noneModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
	ModelFactory::DeleteModel(m_switchModel);
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

	// UI�̏�����
	m_userInterface = std::make_unique<UserInterface>();
	m_sharedSystem = GetSystemManager();
	m_userInterface->Initialize(m_sharedSystem, context, device);

	// ��ʃT�C�Y�̊i�[
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);
	
	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// �����̐ݒ�
	GetSystemManager()->GetString()->CreateString(device, context);

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);
	
	// ���f�����쐬����
	m_noneModel = ModelFactory::GetModel(					// �����S���u���b�N
		device,
		L"Resources/Models/Eraser.cmo"
	);
	m_grassModel = ModelFactory::GetModel(					// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetModel(					// �R�C���u���b�N
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetModel(					// �_�u���b�N
		device,
		L"Resources/Models/Clowd.cmo"
	);
	m_switchModel = ModelFactory::GetModel(					// �X�C�b�`�u���b�N
		device,
		L"Resources/Models/Switch.cmo"
	);
}

//--------------------------------------------------------//
//�f�o�b�O�\��                                            //
//--------------------------------------------------------//
void EditScene::DebugLog(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);

	// �V�[�����̕\��
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"EditScene");

	// �������ݒ�
	wchar_t cam[64];

	// �J�����̃|�W�V����
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().z)
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
	swprintf_s(mos, 64, L"SpherePosition = %f,%f,%f",
		m_cursorPos.x,
		m_cursorPos.y,
		m_cursorPos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	auto mouse = Mouse::Get().GetState();
	// �}�E�X�ʒu�m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"Mouse = %d:%d", mouse.x, mouse.y);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, num);
	
	// �}�E�X�ʒu�m�F
	wchar_t mw[32];
	swprintf_s(mw, 32, L"MouseWheel = %d", mouse.scrollWheelValue);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,100 }, mw);

	// �ʒu�m�F
	wchar_t bs[32];
	swprintf_s(bs, 32, L"BlockState = %d", m_nowState);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,120 }, bs);

	// �o�ߎ���
	wchar_t sec[32];
	swprintf_s(sec, 32, L"Time = %f", m_timer);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,140 }, sec);

	// �J�����̊p�x
	wchar_t ang[32];
	swprintf_s(ang, 32, L"CameraAngle = %f", GetSystemManager()->GetCamera()->GetCameraAngle().x);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,160 }, ang);



	// �f�o�C�X�R���e�L�X�g�̎擾�F�O���b�h�̕`��Ɏg�p
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// �f�o�b�O�O���b�h�̕\��
	GetSystemManager()->GetGridFloor()->Draw(context, GetSystemManager()->GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//�`��{�b�N�X�̃X�e�[�^�X��ύX����                      //
//--------------------------------------------------------//
void EditScene::ChangeState(const int& State)
{
	m_nowState = State;
}

//--------------------------------------------------------//
//�}�b�v��ҏW����                                        //
//--------------------------------------------------------//
void EditScene::EditMap()
{
	auto mouse = Mouse::Get().GetState();

	// �J�������[�h�͏������Ȃ�
	if (m_userInterface->GetCameraFlag()) return;

	// �ړ�����
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// �}�E�X�J�[�\���ňړ�
	m_cursorPos.y = UserUtillity::Lerp(
		m_cursorPos.y,															// �J�n�n�_
		static_cast<float>(mouse.scrollWheelValue / WHEEL_SPAWN) + COMMON_LOW,	// �I���n�_
		0.1f																	// ���x
	);

	// ����������
	m_cursorPos.y = UserUtillity::Clamp(m_cursorPos.y, -2.0f, 15.0f);

	// �}�b�v�Ƃ̓����蔻��
	for (auto& i : m_mapObj)
	{
		// �����߂������𖳌���
		is_boxCol.SetPushMode(false);

		// �����蔻������
		is_boxCol.PushBox(&m_cursorPos, i.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE });

		// �u�Ԃ̓����蔻����擾
		bool hit = is_boxCol.GetHitBoxFlag();
		i.hit = hit;

		if (hit && GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
		{
			i.id = m_nowState;
		}
	}
}

//--------------------------------------------------------//
//���W�̕␳                                              //
//--------------------------------------------------------//
// �ǂݍ��ݎ�
void EditScene::OffsetPosition_Read(std::vector<Object>* obj)
{
	for (auto& i : *obj)
	{
		i.position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		i.position.y += static_cast<float>(COMMON_LOW);
		i.position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}
// �������ݎ�
void EditScene::OffsetPosition_Write(std::vector<Object>* obj)
{
	for (auto& i : *obj)
	{
		i.position.x += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		i.position.y -= static_cast<float>(COMMON_LOW);
		i.position.z += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}

//--------------------------------------------------------//
//�}�b�v�ǂݏ���                                          //
//--------------------------------------------------------//
// �ǂݍ��ݎ�
void EditScene::LoadMap(std::wstring filename)
{ 
	// �}�b�v�̓ǂݍ���
	m_map.LoadMap(filename);

	// �p�X���擾�ł��Ȃ���ΐV�������ʂ̃}�b�v���쐬����
	m_filePath = m_map.GetFilePath();

	// �}�b�v���i�[����
	m_mapObj = m_map.GetMapData();

	// ���W�␳
	OffsetPosition_Read(&m_mapObj);
}
// �������ݎ�
void EditScene::SaveFile()
{	
	// �t�@�C�������o��
	OffsetPosition_Write(&m_mapObj);		// �����o���p�ɍ��W�␳
	m_map.WriteMap(m_mapObj);				// �t�@�C���̏����o��
	OffsetPosition_Read(&m_mapObj);			// �G�f�B�b�g�p�ɍ��W�␳
}