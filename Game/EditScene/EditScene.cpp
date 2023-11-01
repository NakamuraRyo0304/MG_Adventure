/*
 *	@File	EditScene.cpp
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // MementPattern�̎Q�l�T�C�g
 //-------------------------------------------------------------------------------------//
 // �@REFACTORING�EGURU�l
 //   https://refactoring.guru/ja/design-patterns/memento
 //   shiraberu.tech�l
 //   https://shiraberu.tech/2021/11/19/memento-patten/
 //-------------------------------------------------------------------------------------//

// UI
#include "Objects/EditUI.h"

// �}�E�X�J�[�\��
#include "System/MouseCursor.h"

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
	, m_history{}								// ���O�Ǘ�
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
	GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());

	// �ϐ��̏�����
	SetSceneValues();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// ���C�̍X�V
	GetSystemManager()->GetRayCast()->Update();

	// UI�̏���
	m_editUI->Update();

	// �}�E�X�J�[�\���̈ʒu���X�V
	m_mouseCursor->Update();

	// �G�X�P�[�v�ŏI��
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME);}

	// �T�E���h
	auto& _sound = GetSystemManager()->GetSoundManager();

	// �I�����Ă���I�u�W�F�N�g���i�[
	m_nowState = m_editUI->GetNowState();

	// �ҏW�̎�����������
	DoUndoRedo();

	// �Z�[�u�t���O����������t�@�C����ۑ�
	if (m_editUI->GetSaveFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		// �v�f�`�F�b�N���ĕۑ��\�Ȃ���s
		if (IsCanSave())
		{
			_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
			SaveFile();
		}
	}

	// �I�[�v���t���O����������t�@�C�����J��
	if (m_editUI->GetOpenFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		if (!m_mapLoader.LoadMap(L""))
		{
			return;
		}
		m_mapObj = m_mapLoader.GetMapData();	// �ǂݍ���
	}

	// C�L�[����������A�J�������[�h�𔽓]����
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::C))
	{
		// �C���^�[�t�F�[�X�ŃJ�����̃t���O���擾
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		m_editUI->SetCameraFlag(!m_editUI->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());
	}

	// �J�������[�h����Ȃ���ΕҏW�ł���
	if (not m_editUI->GetCameraFlag())
	{
		EditMap();
	}

	// �{�^���N���b�N�ŃZ���N�g�ɖ߂�
	if (m_editUI->GetBackSelectFlag())
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
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
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

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
	SimpleMath::Matrix _rotateY  = SimpleMath::Matrix::CreateRotationY(_timer);
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
	SimpleMath::Matrix _skyMat = SimpleMath::Matrix::CreateRotationY(_timer * SKY_ROTATE_RATE);
	m_skyDomeModel->Draw(_context, _states, _skyMat, _view, _projection);

	// �摜�̕`��
	m_editUI->Render();

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
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �s��v�Z
	SimpleMath::Matrix _rotateY = SimpleMath::Matrix::CreateRotationY(_timer);

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
	GetSystemManager()->CreateUnique();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UI�̏�����
	m_editUI = std::make_unique<EditUI>();
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(_context);
	m_editUI->Create(GetSystemManager(), _device, GetScreenSize());

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(GetScreenSize().x, GetScreenSize().y);

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

	// �ǂݍ��񂾌�Ɉꎞ�ۑ�����
	SaveModification();

	// �J�����̈ʒu���}�b�v�̒��S�ɂ���
	SimpleMath::Vector2 _XZ = { m_mapLoader.MAP_COLUMN,m_mapLoader.MAP_RAW };
	GetSystemManager()->GetCamera()->AddEyePosition(SimpleMath::Vector3{ _XZ.x / 2,4.0f,_XZ.y / 2 });
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
		m_cursorPos.y,												// �J�n�n�_
		static_cast<float>((_mouse.scrollWheelValue) / WHEEL_SPAWN),// �I���n�_
		CURSOR_MOVE_SPEED											// ���x
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

			// �ꎞ�ۑ�����
			SaveModification();

			// �I�u�W�F�N�g���Z�b�g
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SETBOX, false);
			i.id = m_nowState;
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
}

/// <summary>
/// �}�b�v�����o��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::SaveFile()
{
	m_mapLoader.WriteMap(m_mapObj);		// �t�@�C���̏����o��
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

/// <summary>
/// UndoRedo�����s����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::DoUndoRedo()
{
	auto& _input = Input::GetInstance();

	// �O�ɖ߂�
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Z))
	{
		RestoreHistory(m_history.GetUndo());
	}
	// Undo��������
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::X))
	{
		RestoreHistory(m_history.GetRedo());
	}
}

/// <summary>
/// UndoRedo�p�ɕۑ�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void EditScene::SaveModification()
{
	// ��Ԃ�ۑ�
	m_history.AddHistory(MementoMap(m_mapObj));
}

/// <summary>
/// UndoRedo��K�p����
/// </summary>
/// <param name="mement"></param>
/// <returns>�Ȃ�</returns>
void EditScene::RestoreHistory(MementoMap mement)
{
	m_mapObj = mement.GetState();
}
