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

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
EditScene::EditScene() :
	IScene(),
	m_timer{0.0f},					// �^�C�}�[
	m_userInterface{},				// UI
	m_cursorPos{0.0f,0.0f,0.0f},	// �J�[�\���̈ʒu
	m_mapObj{0},					// �i�[�z��
	m_nowState{},					// ���݂̃u���b�N�̎��
	m_map{},						// �}�b�v
	is_boxCol{},					// �����̓����蔻��
	m_grassModel{ nullptr },		// ���f��
	m_noneModel{ nullptr },			// |
	m_coinModel{ nullptr },			// |
	m_clowdModel{ nullptr },		// |
	m_reClowdPtModel{ nullptr },	// |
	m_skyDomeModel{ nullptr },		// |
	m_sharedSystem{}				// �V�X�e���f�[�^
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
EditScene::~EditScene()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ���L���ɂ���
	GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());

	// �}�b�v�ǂݍ���//����ǂݍ���
	LoadMap(L"Resources/Maps/Stage1.csv");

	// �����l�͑��u���b�N
	m_nowState = MapState::GrassBox;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">����/fps</param>
/// <param name="keyState">�L�[�{�[�h�|�C���^</param>
/// <param name="mouseState">�}�E�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
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

	m_nowState = m_userInterface->GetNowState();

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
		m_map.LoadMap(L"");						// �t�@�C���V�K�쐬
		m_mapObj = m_map.GetMapData();			// �ǂݍ���
		OffsetPosition(&m_mapObj,READ);			// ���W�␳
	}

	// �V�t�g�������Ă���Ԃ̓J�������[�h��True�ɂ���
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::LeftShift))
	{
		// �C���^�[�t�F�[�X�ŃJ�����̃t���O���擾
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}


	// �J�������[�h����Ȃ���ΕҏW�ł���
	if (m_userInterface->GetCameraFlag() == false)
	{
		EditMap();
	}

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix world, view, proj;

	// ���[���h�s��
	world = SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// ���C�̐ݒ�
	GetSystemManager()->GetRayCast()->SetMatrix(view, proj);

	// �s��v�Z
	SimpleMath::Matrix scale = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix boxMat =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// �I�𒆃I�u�W�F
		if (m_mapObj[i].id == MapState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapState::CoinBox)
		{
			m_coinModel->Draw(context, states, rotY * boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapState::ClowdBox)
		{
			m_clowdModel->Draw(context, states, rotY * boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapState::ResetClowd)
		{
			m_reClowdPtModel->Draw(context, states, rotY * boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapState::PlayerPos)
		{
			m_playerModel->Draw(context, states, rotY * boxMat, view, proj);
		}
	}

	// �T�C�Y�@�~�@��]�@�~�@�ړ�
	world *= scale *  rotY * trans;

	// �I�����Ă���I�u�W�F��`��
	switch (m_nowState)
	{
	case MapState::GrassBox:	// ��
		m_grassModel->Draw(context, states, world, view, proj);
		break;
	case MapState::CoinBox:		// �R�C��
		m_coinModel->Draw(context, states, world, view, proj);
		break;
	case MapState::ClowdBox:	// �_
		m_clowdModel->Draw(context, states, world, view, proj);
		break;
	case MapState::ResetClowd:	// �_���Z�b�g
		m_reClowdPtModel->Draw(context, states, world, view, proj);
		break;
	case MapState::PlayerPos:	// �v���C���[
		m_playerModel->Draw(context, states, world, view, proj);
		break;
	case MapState::None:		// �����S��
		m_noneModel->Draw(context, states, world, view, proj);
		break;
	default:
		break;
	}

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer / 10);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);

	// �摜�̕`��
	m_userInterface->Render();
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// ���f���̔j��
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
	ModelFactory::DeleteModel(m_reClowdPtModel);
	ModelFactory::DeleteModel(m_playerModel);
	ModelFactory::DeleteModel(m_skyDomeModel);
	ModelFactory::DeleteModel(m_noneModel);
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);

	// ��ʃT�C�Y�̊i�[
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// UI�̏�����
	m_userInterface = std::make_unique<UserInterface>(SimpleMath::Vector2(width, height));
	m_sharedSystem = GetSystemManager();
	m_userInterface->Initialize(m_sharedSystem, context, device);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// �����̐ݒ�
	GetSystemManager()->GetString()->CreateString(device, context);

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);

	// ���f�����쐬����
	m_grassModel = ModelFactory::GetCreateModel(		// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetCreateModel(			// �R�C���u���b�N
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetCreateModel(		// �_�u���b�N
		device,
		L"Resources/Models/MoveBlock.cmo"
	);
	m_reClowdPtModel = ModelFactory::GetCreateModel(	// �_���Z�b�g
		device,
		L"Resources/Models/ResetPt.cmo"
	);
	m_playerModel = ModelFactory::GetCreateModel(		// �v���C���u���b�N
		device,
		L"Resources/Models/Bird.cmo"
	);
	m_noneModel = ModelFactory::GetCreateModel(			// �����S���u���b�N
		device,
		L"Resources/Models/Eraser.cmo"
	);
		// �X�J�C�h�[�����f�����쐬����
	m_skyDomeModel = ModelFactory::GetCreateModel(
		device,
		L"Resources/Models/Spacedome.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			// ���Ȕ�������
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::Blue);
			}
		}
	);
}

/// <summary>
/// �}�b�v�̕ҏW
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::EditMap()
{
	auto mouse = Mouse::Get().GetState();

	// �J�������[�h�͏������Ȃ�
	if (m_userInterface->GetCameraFlag()) return;

	// �ړ�����
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// �}�E�X�J�[�\���ňړ�
	m_cursorPos.y = UserUtility::Lerp(
		m_cursorPos.y,															// �J�n�n�_
		static_cast<float>(mouse.scrollWheelValue / WHEEL_SPAWN) + COMMON_LOW,	// �I���n�_
		0.1f																	// ���x
	);

	// ����������
	m_cursorPos.y = UserUtility::Clamp(m_cursorPos.y, CURSOR_MIN, CURSOR_MAX);

	// �}�b�v�Ƃ̓����蔻��
	for (auto& obj : m_mapObj)
	{
		// �����߂������𖳌���
		is_boxCol.SetPushMode(false);

		// �����蔻������
		is_boxCol.PushBox(&m_cursorPos, obj.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		// �u�Ԃ̓����蔻����擾
		obj.hit = is_boxCol.GetHitBoxFlag();

		// �N���b�N�Ńu���b�N�ݒu
		if (obj.hit && GetSystemManager()->GetMouseTrack()->leftButton ==
			Mouse::ButtonStateTracker::RELEASED)
		{
			obj.id = m_nowState;
		}
	}
}

/// <summary>
/// ���W�␳
/// </summary>
/// <param name="object">�}�b�v�f�[�^</param>
/// <param name="mode">�ǂݏ������[�h</param>
/// <returns>�Ȃ�</returns>
void EditScene::OffsetPosition(std::vector<Object>* object, const int& mode)
{
	// �ǂݍ���
	if (mode == READ)
	{
		for (auto& obj : *object)
		{
			obj.position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
			obj.position.y += static_cast<float>(COMMON_LOW);
			obj.position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
	// ��������
	if (mode == WRITE)
	{
		for (auto& i : *object)
		{
			i.position.x += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y -= static_cast<float>(COMMON_LOW);
			i.position.z += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
}

/// <summary>
/// �}�b�v�ǂݍ���
/// </summary>
/// <param name="filename">�t�@�C���p�X</param>
/// <returns>�Ȃ�</returns>
void EditScene::LoadMap(std::wstring filename)
{
	// �}�b�v�̓ǂݍ���
	m_map.LoadMap(filename);

	// �p�X���擾�ł��Ȃ���ΐV�������ʂ̃}�b�v���쐬����
	m_filePath = m_map.GetFilePath();

	// �}�b�v���i�[����
	m_mapObj = m_map.GetMapData();

	// ���W�␳
	OffsetPosition(&m_mapObj,READ);
}

/// <summary>
/// �}�b�v�����o��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::SaveFile()
{
	// �t�@�C�������o��
	OffsetPosition(&m_mapObj,WRITE);	// �����o���p�ɍ��W�␳
	m_map.WriteMap(m_mapObj);			// �t�@�C���̏����o��
	OffsetPosition(&m_mapObj,READ);		// �ǂݍ��ݗp�ɍ��W�␳
}