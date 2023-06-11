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

// �摜�̒��S�ʒu
#define			IMAGE_CENTER		128

// �{�b�N�X�̍ő�l�ƍŏ��l
#define			MAX_BOX				15
#define			MIN_BOX				1


 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_sphere{},						// ��
	m_spherePos{0.0f,COMMON_LOW ,0.0f},
	m_box{},						// ��
	m_mapObj{},						// �i�[�z��
	m_grassObj{},
	m_nowState{},					// ���݂̃u���b�N�̎��
	m_map{},						// �}�b�v
	m_boxCol{},						// �����̓����蔻��
	m_grassBlockModel{ nullptr },	// ���f��
	m_grassBlockModel_D{ nullptr },	
	m_coinModel{ nullptr },
	m_saveTexPos{},					// �摜���W
	m_cameraTexPos{},
	m_penTexPos{},
	is_saveFlag{},					// �I���t���O
	is_cameraFlag{true},			// �J�������[�hON�ŃX�^�[�g
	is_upFlag{true}					// �y�����[�h�ŃX�^�[�g			
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

	GetSystemManager()->GetCamera()->SetMoveMode(false);				// �J�������W�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(is_cameraFlag);		// �J�������_�ړ�

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
	LoadMap();

	// �����l�͑��u���b�N
	m_nowState = MapLoad::BoxState::GrassBox;
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

	// UI�̏���
	ClickUserInterface(mouseState);
	
	// �X�e�[�^�X�ύX
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Right))
	{
		switch (m_nowState)
		{
		case MapLoad::BoxState::GrassBox:
			ChangeState(MapLoad::BoxState::CoinBox);
			break;
		case MapLoad::BoxState::CoinBox:
			ChangeState(MapLoad::BoxState::ClowdBox);
			break;
		case MapLoad::BoxState::ClowdBox:
			ChangeState(MapLoad::BoxState::GrassBox);
			break;
		default:
			break;
		}
	}

	// �J�������[�h����Ȃ���ΕҏW�ł���
	if (!is_cameraFlag)
	{
		EditMap();
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
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

	// ��J�������[�h�̂ݕ`��
	if (!is_cameraFlag)
	{
		m_sphere->Draw(world, view, projection, DirectX::Colors::Red);
	}

	// ���I�u�W�F�N�g�̕`��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_grassObj[y][x].state % 100; h++)
			{
				m_grassObj[y][x].position.y = COMMON_LOW + h * COMMON_SIZE;

				// �{�b�N�X�̈ړ�
				DirectX::SimpleMath::Matrix boxWorldMat =
					DirectX::SimpleMath::Matrix::CreateTranslation(m_grassObj[y][x].position);

				if (m_grassObj[y][x].state % 100 == 0) return;	// �{�b�N�X���Ȃ���Ε`�悵�Ȃ�
				
				// �`�揈��
				if (m_grassObj[y][x].hitFlag && m_grassObj[y][x].state - m_grassObj[y][x].state % 100 == MapLoad::BoxState::GrassBox)
				{
					m_grassBlockModel_D->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
				}
				else
				{
					// ���u���b�N
					if (m_grassObj[y][x].state - m_grassObj[y][x].state % 100 == MapLoad::BoxState::GrassBox)
					{
						m_grassBlockModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
							*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
					}
					// �R�C���u���b�N
					if (m_grassObj[y][x].state - m_grassObj[y][x].state % 100 == MapLoad::BoxState::CoinBox)
					{
						m_coinModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
							*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
					}
				}
			}
		}
	}

	// �摜�̕`��
	DrawImages();

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
	m_grassBlockModel = ModelFactory::GetModel(					// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_grassBlockModel_D = ModelFactory::GetModel(				// ���I���u���b�N
		device,
		L"Resources/Models/GrassBlock_Dark.cmo"
	);
	m_coinModel = ModelFactory::GetModel(						// �R�C���u���b�N
		device,
		L"Resources/Models/Coin.cmo"

	);

	// �摜�̐ݒ�
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(context);
	// �L�[���@�F�@�t�@�C���p�X���@�F�@�f�o�C�X
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Pen", L"Resources/Textures/AddBlock.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);

	// ���W���
	m_saveTexPos   = { width - 100, 80 };
	m_cameraTexPos = { width - 100,208 };
	m_penTexPos    = { width - 100,336 };
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
	swprintf_s(mos, 64, L"SpherePosition = %f,%f,%f",
		m_spherePos.x,
		m_spherePos.y,
		m_spherePos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	auto mouse = DirectX::Mouse::Get().GetState();
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
	auto mouse = DirectX::Mouse::Get().GetState();

	// �ړ�����
	m_spherePos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_spherePos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;
	
	if (!is_cameraFlag)
	{
		m_spherePos.y = UserUtillity::Lerp(m_spherePos.y, mouse.scrollWheelValue / 640.0f + COMMON_LOW, 0.1f);
	}

	// �����蔻��
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_grassObj[y][x].state % 100; h++)
			{
				m_grassObj[y][x].position.y = COMMON_LOW + h * COMMON_SIZE;

				m_boxCol.PushBox(&m_spherePos, m_grassObj[y][x].position,					// �I�u�W�F�N�g
					DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2 },						// �T�C�Y
					DirectX::SimpleMath::Vector3{ COMMON_SIZE }								// �T�C�Y
				);

				// �����Ă�����True�ɂ���
				m_grassObj[y][x].hitFlag = m_boxCol.GetHitBoxFlag();

				// ���N���b�N�Ńu���b�N�̒ǉ����폜
				if (m_grassObj[y][x].hitFlag &&
					GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED &&
					is_upFlag)
				{
					m_grassObj[y][x].state += 1;
				}
				if (m_grassObj[y][x].hitFlag &&
					GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED &&
					!is_upFlag)
				{
					m_grassObj[y][x].state -= 1;
				}

				// �N�����v����
				ClampHeight(m_grassObj[y][x].state, m_nowState);
			}
		}
	}
}

//--------------------------------------------------------//
//�N�����v����                                            //
//--------------------------------------------------------//
void EditScene::ClampHeight(int& states, int id)
{
	int temp = states - id;
	temp = UserUtillity::Clamp(temp, MIN_BOX, MAX_BOX);
	states = id + temp;
}

//--------------------------------------------------------//
//�}�b�v�ǂݍ���                                          //
//--------------------------------------------------------//
void EditScene::LoadMap()
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
			// �z��̂��݂�����
			m_grassObj[y][x].position = DirectX::SimpleMath::Vector3::Zero;

			m_grassObj[y][x].state = m_map.GetMapData(x,y);

			// �{�b�N�X�̈ʒu��������
			m_grassObj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),		// �u���b�N�̈ʒu - �I�t�Z�b�g
				COMMON_LOW + COMMON_SIZE,									// �u���b�N�̍Œፂ�x
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// �u���b�N�̈ʒu - �I�t�Z�b�g
			};
		}
	}
}

//--------------------------------------------------------//
//�t�@�C������������                                      //
//--------------------------------------------------------//
void EditScene::SaveFile()
{
	// ���e���L�^
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_map.SetMapData(m_mapObj[y][x].state, x, y);
		}
	}
	// �t�@�C�������o��
	m_map.WriteMap();
}

//--------------------------------------------------------//
//�摜�̕`��                                              //
//--------------------------------------------------------//
void EditScene::DrawImages()
{
	// �t�@�C���A�C�R��
	if (is_saveFlag)
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Save",							// �o�^�L�[
			m_saveTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
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
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"CameraMove",						// �o�^�L�[
			m_cameraTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Camera",							// �o�^�L�[
			m_cameraTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,0.3f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}

	// �y���A�C�R��
	if (is_upFlag)
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Pen",								// �o�^�L�[
			m_penTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Erase",							// �o�^�L�[
			m_penTexPos,						// ���W
			{ 1.0f,1.0f,1.0f,1.0f },			// �F
			0.5f,								// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }		// ���S�ʒu
		);
	}
}

//--------------------------------------------------------//
//UI�̃N���b�N                                            //
//--------------------------------------------------------//
void EditScene::ClickUserInterface(DirectX::Mouse::State& mouseState)
{
	// �ۑ��A�C�R�����N���b�N
	is_saveFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_saveTexPos.x,0,m_saveTexPos.y },		        // �摜�̈ʒu
		DirectX::SimpleMath::Vector3{ 5.0f },		    // �T�C�Y
		DirectX::SimpleMath::Vector3{ 100.0f });	    // �T�C�Y
	// �����Ă��ăN���b�N����������
	if (is_saveFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_saveFlag = true;
		// �_�C�A���O��\�����ă}�b�v���o��
		SaveFile();
	}

	// �J�����A�C�R�����N���b�N
	bool cameraFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_cameraTexPos.x,0,m_cameraTexPos.y },	 	// �摜�̈ʒu
		DirectX::SimpleMath::Vector3{ 5.0f },		    // �T�C�Y
		DirectX::SimpleMath::Vector3{ 100.0f });	    // �T�C�Y

	// �J�����ړ����[�h�؂�ւ�
	if (cameraFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_cameraFlag = !is_cameraFlag;
		auto camera = GetSystemManager()->GetCamera();
		camera->SetEagleMode(is_cameraFlag);
	}

	// �y��/�����S���A�C�R�����N���b�N
	bool toolFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// �}�E�X�̈ʒu
		{ m_penTexPos.x,0,m_penTexPos.y },	        	// �摜�̈ʒu
		DirectX::SimpleMath::Vector3{ 5.0f },		    // �T�C�Y
		DirectX::SimpleMath::Vector3{ 100.0f });	    // �T�C�Y

	// �`�惂�[�h�؂�ւ�
	if (toolFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_upFlag = !is_upFlag;
	}
}
