/*
 *	@File	EditScene.cpp
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#include "pch.h"

// UI
#include "Objects/EditUI.h"

// �}�E�X�J�[�\��
#include "../../Libraries/SystemDatas/MouseCursor.h"

// �N���A��Փx�`�F�b�J�[
#include "System/ClearChecker.h"

#include "EditScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
EditScene::EditScene()
	: IScene()									// ���N���X�̏�����
	, m_timer{ 0.0f }							// �^�C�}�[
	, m_mapLoader{}								// �}�b�v���[�_�[
	, m_mapObj{0}								// �i�[�z��
	, m_nowState{}								// ���݂̃u���b�N�̎��
	, m_grassModel{ nullptr }					// ���f���Q��
	, m_noneModel{ nullptr }					// ���f���Q�����S��
	, m_coinModel{ nullptr }					// ���f���Q�R�C��
	, m_cloudModel{ nullptr }					// ���f���Q�_
	, m_gravityModel{ nullptr }					// ���f���Q�d��
	, m_skyDomeModel{ nullptr }					// ���f���Q�X�J�C�h�[��
	, m_cursorPos{ SimpleMath::Vector3::Zero }	// �J�[�\���̈ʒu
	, is_hitCol{}								// �����̓����蔻��
{
	ShowCursor(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
EditScene::~EditScene()
{
	Finalize();
	ShowCursor(true);
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

	// �ϐ��̏�����
	SetSceneValues();
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

	// �}�E�X�J�[�\���̈ʒu���X�V
	m_mouseCursor->Update(SimpleMath::Vector2{ static_cast<float>(mouseState.x),static_cast<float>(mouseState.y) });

	// �G�X�P�[�v�ŏI��
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();

	// �I�����Ă���I�u�W�F�N�g���i�[
	m_nowState = m_userInterface->GetNowState();

	// �Z�[�u�t���O����������t�@�C����ۑ�
	if (m_userInterface->GetSaveFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		// �v�f�`�F�b�N���ĕۑ��\�Ȃ���s
		if (IsCanSave())
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
			SaveFile();
		}
	}

	// �I�[�v���t���O����������t�@�C�����J��
	if (m_userInterface->GetOpenFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		if (!m_mapLoader.LoadMap(L""))
		{
			return;
		}
		m_mapObj = m_mapLoader.GetMapData();	// �ǂݍ���
		OffsetPosition(&m_mapObj,READ);			// ���W�␳
	}

	// Z�L�[����������A�J�������[�h�𔽓]����
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Z))
	{
		// �C���^�[�t�F�[�X�ŃJ�����̃t���O���擾
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}

	// �J�������[�h����Ȃ���ΕҏW�ł���
	if (m_userInterface->GetCameraFlag() == false)
	{
		EditMap();
	}

	// �{�^���N���b�N�ŃZ���N�g�ɖ߂�
	if (m_userInterface->GetBackSelectFlag())
	{
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		ChangeScene(SCENE::SELECT);
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
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix _cursorMat, _view, _projection;

	// �r���[�s��
	_view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// ���C�̐ݒ�
	GetSystemManager()->GetRayCast()->SetMatrix(_view, _projection);

	// �s��v�Z
	SimpleMath::Matrix _scale	 = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix _rotateY  = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix _trans	 = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// �T�C�Y�@�~�@��]�@�~�@�ړ�
	_cursorMat *= _scale *  _rotateY * _trans;

	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix _boxMat =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// �����߂������𖳌���
		is_hitCol.SetPushMode(false);

		// �����蔻������
		is_hitCol.PushBox(&m_cursorPos, m_mapObj[i].position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		m_mapObj[i].hit = is_hitCol.IsHitBoxFlag();

		if (m_mapObj[i].hit) // �I�𒆂̃}�X�ɃI�u�W�F�N�g��`��
		{
			SwitchDraw(m_nowState, _context, _states, _boxMat, _view, _projection);
		}
		else				 // �Y���I�u�W�F�N�g�̕`��
		{
			SwitchDraw(m_mapObj[i].id, _context, _states, _boxMat, _view, _projection);
		}
	}

	// �}�E�X�ʒu�ɕ`��
	if (m_nowState == MAPSTATE::NONE) // �폜���ȊO�͒ʏ�̕`��
	{
		m_noneModel->Draw(_context, _states, _cursorMat, _view, _projection);
	}
	else
	{
		SwitchDraw(m_nowState, _context, _states, _cursorMat, _view, _projection);
	}

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix _skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROTATE_RATE);
	m_skyDomeModel->Draw(_context, _states, _skyMat, _view, _projection);

	// �摜�̕`��
	m_userInterface->Render();

	// �}�E�X�J�[�\���̕`��
	m_mouseCursor->Render();
}

/// <summary>
/// �`��I�u�W�F�N�g�؂�ւ�
/// </summary>
/// <param name="objNum">�I�u�W�F�N�g�ԍ�</param>
/// <param name="context">�f�o�C�X�R���e�L�X�g</param>
/// <param name="states">�R�����X�e�[�g</param>
/// <param name="world">���[���h�s��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void EditScene::SwitchDraw(const int& objNum, ID3D11DeviceContext* context,	CommonStates& states,
	SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// �s��v�Z
	SimpleMath::Matrix _rotateY = SimpleMath::Matrix::CreateRotationY(m_timer);

	switch (objNum)
	{
	case MAPSTATE::GRASS:	// ��
		m_grassModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::COIN:	// �R�C��
		m_coinModel->Draw(context, states, _rotateY * world, view, proj);
		break;
	case MAPSTATE::CLOUD:	// �_
		m_cloudModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::GRAVITY:	// �d��
		m_gravityModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::PLAYER:	// �v���C���[
		m_playerModel->Draw(context, states, _rotateY * world, view, proj);
		break;
	default:
		break;
	}
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
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_gravityModel);
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
	auto _device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(_device, _context);

	// ��ʃT�C�Y�̊i�[
	float _width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float _height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// UI�̏�����
	m_userInterface = std::make_unique<EditUI>(SimpleMath::Vector2(_width, _height));
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(_context);
	m_userInterface->Initialize(GetSystemManager(), _device);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(_width, _height, CAMERA_ANGLE);

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(_width, _height);

	// ���f�����쐬����
	m_grassModel = ModelFactory::GetCreateModel(		// ���u���b�N
		_device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetCreateModel(			// �R�C���u���b�N
		_device,
		L"Resources/Models/Coin.cmo"
	);
	m_cloudModel = ModelFactory::GetCreateModel(		// �_�u���b�N
		_device,
		L"Resources/Models/Cloud.cmo"
	);
	m_gravityModel = ModelFactory::GetCreateModel(		// �d�̓u���b�N
		_device,
		L"Resources/Models/ResetPt.cmo"
	);
	m_playerModel = ModelFactory::GetCreateModel(		// �v���C���u���b�N
		_device,
		L"Resources/Models/Bird.cmo"
	);
	m_noneModel = ModelFactory::GetCreateModel(			// �����S���u���b�N
		_device,
		L"Resources/Models/Eraser.cmo"
	);
	m_skyDomeModel = ModelFactory::GetCreateModel(		// �X�J�C�h�[��
		_device,
		L"Resources/Models/EditSky.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ���C�g�̐�����
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, false);
				}
			}
			// ���Ȕ�������
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// �}�E�X�J�[�\���̍쐬
	m_mouseCursor = std::make_unique<MouseCursor>(_context);
	m_mouseCursor->Initialize(L"Resources/Textures/EDITS/MouseCursor.dds", _device);

	// �N���A�`�F�b�J�[�̍쐬
	m_checker = std::make_unique<ClearChecker>();
}

/// <summary>
/// �V�[�����̕ϐ��������֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::SetSceneValues()
{
	// �����l�͑��u���b�N
	m_nowState = MAPSTATE::GRASS;

	// �}�b�v�ǂݍ���//����ǂݍ���
	LoadMap(L"Datas/Maps/StageEdit.csv");

	// �N���A�`�F�b�J�[�ɔz���n��
	m_checker->SetMap(m_mapObj);

	GetSystemManager()->GetCamera()->SetAddPosition(SimpleMath::Vector3{ 0.0f,5.0f,0.0f });
}

/// <summary>
/// �}�b�v�̕ҏW
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::EditMap()
{
	auto _mouse = Mouse::Get().GetState();

	// �ړ�����
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// �}�E�X�J�[�\���ňړ�
	m_cursorPos.y = UserUtility::Lerp(
		m_cursorPos.y,																// �J�n�n�_
		static_cast<float>((_mouse.scrollWheelValue) / WHEEL_SPAWN) + COMMON_LOW,	// �I���n�_
		CURSOR_MOVE_SPEED															// ���x
	);

	// ����������
	m_cursorPos.y = UserUtility::Clamp(m_cursorPos.y, CURSOR_MIN, CURSOR_MAX);

	// �}�b�v�Ƃ̓����蔻��
	for (auto& i : m_mapObj)
	{
		// �����߂������𖳌���
		is_hitCol.SetPushMode(false);

		// �����蔻������
		is_hitCol.PushBox(&m_cursorPos, i.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		// �u�Ԃ̓����蔻����擾
		i.hit = is_hitCol.IsHitBoxFlag();

		// �N���b�N�Ńu���b�N�ݒu
		if (i.hit &&  _mouse.leftButton)
		{
			// ���ɓ����I�u�W�F�N�g�Ȃ珈�����Ȃ�
			if (i.id == m_nowState) continue;

			// �I�u�W�F�N�g���Z�b�g
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SETBOX, false);
			i.id = m_nowState;
		}
	}
}

/// <summary>
/// ���W�␳
/// </summary>
/// <param name="object">�}�b�v�f�[�^</param>
/// <param name="mode">�ǂݏ������[�h</param>
/// <returns>�Ȃ�</returns>
void EditScene::OffsetPosition(std::vector<Object>* object, const MODE& mode)
{
	// �ǂݍ���
	if (mode == READ)
	{
		for (auto& i : *object)
		{
			i.position.x -= static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y += static_cast<float>(COMMON_LOW);
			i.position.z -= static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
	// ��������
	if (mode == WRITE)
	{
		for (auto& i : *object)
		{
			i.position.x += static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y -= static_cast<float>(COMMON_LOW);
			i.position.z += static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
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
	m_mapLoader.LoadMap(filename);

	// �p�X���擾�ł��Ȃ���ΐV�������ʂ̃}�b�v���쐬����
	m_filePath = m_mapLoader.GetFilePath();

	// �}�b�v���i�[����
	m_mapObj = m_mapLoader.GetMapData();

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
	m_mapLoader.WriteMap(m_mapObj);		// �t�@�C���̏����o��
	OffsetPosition(&m_mapObj,READ);		// �ǂݍ��ݗp�ɍ��W�␳
}

/// <summary>
/// �Z�[�u�\�����m�F����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
bool EditScene::IsCanSave()
{
	// �J�[�\��������
	ShowCursor(true);

	// �N���A�`�F�b�J�[�ɔz���n��
	m_checker->SetMap(m_mapObj);
	if (m_checker->RunCheck())
	{
		// �N���A���₷�����`�F�b�N���A�����Όx�����o��
		if (!m_checker->GetClearPossibility())
		{
			if (MessageBox(NULL,
				TEXT("�K�v�����͖������Ă��܂����A\n�R�C���̐����������A����Ă��邽�߃N���A������ȉ\��������܂��B"),
				TEXT("����"), MB_YESNO) == IDNO)
			{
				return false;
			}
		}

		// �J�[�\��������
		ShowCursor(false);

		// �v���𖞂����Ă�����True
		return true;
	}
	else
	{
		if (m_checker->GetPlayerNum() > 1)
		{
			MessageBox(NULL, TEXT("�v���C���[�̐����������܂��B"),
				TEXT("��"), MB_OK);
		}
		else if (m_checker->GetPlayerNum() < 1)
		{
			MessageBox(NULL, TEXT("�v���C���[��ݒu���Ă��������B"),
				TEXT("��"), MB_OK);
		}
		if (m_checker->GetCoinNum() < 1)
		{
			MessageBox(NULL, TEXT("�R�C����ݒu���Ă��������B"),
				TEXT("��"), MB_OK);
		}
		if (m_checker->GetCloudFlag() == false)
		{
			MessageBox(NULL, TEXT("�S�Ẳ_�̈ʒu���v���C���[��荂���ł��B\n��ɏオ��ɂ͉��ɐݒu����K�v������܂��B"),
				TEXT("��"), MB_OK);
		}
		if (m_checker->GetCanStart() == false)
		{
			MessageBox(NULL, TEXT("�v���C���[�̉��Ƀu���b�N���Ȃ����߁A�X�^�[�g�ł��܂���B"),
				TEXT("��"), MB_OK);
		}

		// �J�[�\��������
		ShowCursor(false);

		// �v���𖞂����Ă��Ȃ����False
		return false;
	}
}
