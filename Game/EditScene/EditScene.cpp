/*
 *	@File	EditScene.cpp
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Objects/EditUI.h"
#include "Objects/EditSky.h"
#include "System/MouseCursor.h"
#include "System/ClearChecker.h"
#include "EditScene.h"

// MementPattern�̎Q�l�T�C�g
//-------------------------------------------------------------------------------------//
// �@REFACTORING�EGURU�l
//   https://refactoring.guru/ja/design-patterns/memento
//   shiraberu.tech�l
//   https://shiraberu.tech/2021/11/19/memento-patten/
//-------------------------------------------------------------------------------------//

// �R���X�g���N�^
EditScene::EditScene()
	: IScene()									// ���N���X�̏�����
	, m_mapLoader{}								// �}�b�v���[�_�[
	, m_mapObj{0}								// �i�[�z��
	, m_nowState{}								// ���݂̃u���b�N�̎��
	, m_blockCount{0}							// �I�u�W�F�N�g�̐�
	, m_grassModel{ nullptr }					// ��
	, m_noneModel{ nullptr }					// �����S��
	, m_coinModel{ nullptr }					// �R�C��
	, m_cloudModel{ nullptr }					// �_
	, m_gravityModel{ nullptr }					// �d��
	, m_cursorPos{ SimpleMath::Vector3::Zero }	// �J�[�\���̈ʒu
	, m_history{}								// ���O�Ǘ�
	, m_XZCheck{}								// XZ�̃u���b�N�̈ʒu�����߂�
	, is_hitCol{}								// �����̓����蔻��
{
	ShowCursor(false);
}

// �f�X�g���N�^
EditScene::~EditScene()
{
	Finalize();
	ShowCursor(true);
}

// ����������
void EditScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �ϐ��̏�����
	SetSceneValues();
}

// �X�V����
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

	// �G�X�P�[�v�ŃZ���N�g�ɖ߂�
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::SELECT);}

	// �T�E���h
	auto& _sound = GetSystemManager()->GetSoundManager();

	// �I�����Ă���I�u�W�F�N�g���i�[
	m_nowState = m_editUI->GetNowState();

	// �ҏW�̎�����������
	DoUndoRedo();

	// ���f���̃J�E���g
	if (_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		ResetObjNum();
		CountObjNum();
	}

	// �A�C�R���N���b�N�̍X�V
	ClickIcons();

	// C�L�[����������A�J�������[�h�𔽓]����
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::C))
	{
		// �C���^�[�t�F�[�X�ŃJ�����̃t���O���擾
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		m_editUI->SetCameraFlag(!m_editUI->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());
	}
}

// �`�揈��
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
			SwitchDraw(m_nowState, _context, _states, _rotateY * _boxMat, _view, _projection);
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
	m_skyDome->Draw(_states, _view, _projection, _timer);

	// �摜�̕`��
	m_editUI->Render();

	// �f�o�b�O����\��
	GetSystemManager()->GetString()->DrawFormatString(
		_states, { 25,200 }, Colors::Yellow,SimpleMath::Vector2(1.5f),
		L"Grass:%d\nCoin:%d\nCloud:%d\nGravity:%d\nPlayer:%d",
		m_blockCount[MAPSTATE::GRASS],
		m_blockCount[MAPSTATE::COIN],
		m_blockCount[MAPSTATE::CLOUD],
		m_blockCount[MAPSTATE::GRAVITY],
		m_blockCount[MAPSTATE::PLAYER]
	);

	// �}�E�X�J�[�\���̕`��
	m_mouseCursor->Render();
}

// �`��I�u�W�F�N�g�؂�ւ�
void EditScene::SwitchDraw(const int& objNum, ID3D11DeviceContext* context,	CommonStates& states,
	SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	switch (objNum)
	{
	case MAPSTATE::GRASS:	// ��
		m_grassModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::COIN:	// �R�C��
		m_coinModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::CLOUD:	// �_
		m_cloudModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::GRAVITY:	// �d��
		m_gravityModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::PLAYER:	// �v���C���[
		m_playerModel->Draw(context, states, world, view, proj);
		break;
	default:
		break;
	}
}

// �����
void EditScene::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// ���f���̔j��
	auto& _mf = GetFactoryManager();
	_mf->BuildModelFactory();

	_mf->VisitModelFactory()->DeleteModel(m_grassModel);
	_mf->VisitModelFactory()->DeleteModel(m_coinModel);
	_mf->VisitModelFactory()->DeleteModel(m_cloudModel);
	_mf->VisitModelFactory()->DeleteModel(m_gravityModel);
	_mf->VisitModelFactory()->DeleteModel(m_playerModel);
	_mf->VisitModelFactory()->DeleteModel(m_noneModel);

	_mf->LeaveModelFactory();
}

// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
void EditScene::CreateWindowDependentResources()
{
	// �V�X�e���̍쐬
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UI�̏�����
	m_editUI = std::make_unique<EditUI>();
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_editUI->Create(GetSystemManager(),GetScreenSize());

	// ���C���y�Ԕ͈͂�ݒ�
	GetSystemManager()->GetRayCast()->SetScreenSize(GetScreenSize().x, GetScreenSize().y);

	// ���f�����쐬����
	GetFactoryManager()->BuildModelFactory();
	CreateModels(GetFactoryManager());
	GetFactoryManager()->LeaveModelFactory();

	// �X�J�C�h�[���̍쐬
	m_skyDome = std::make_unique<EditSky>(GetFactoryManager(), L"Resources/Models/EditSky.cmo");

	// �}�E�X�J�[�\���̍쐬
	m_mouseCursor = std::make_unique<MouseCursor>();
	m_mouseCursor->Initialize(L"Resources/Textures/EDITS/MouseCursor.dds");

	// �N���A�`�F�b�J�[�̍쐬
	m_checker = std::make_unique<ClearChecker>();
}

// �V�[���ϐ��������֐�
void EditScene::SetSceneValues()
{
	// �J�������_�ړ���L���ɂ���
	GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());

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
	GetSystemManager()->GetCamera()->AddEyePosition(SimpleMath::Vector3{ _XZ.x / 2,3.0f,_XZ.y / 2 });

	// XZ�ۑ��ϐ���������
	m_XZCheck = SimpleMath::Vector3::Zero;
}

// ���f���̍쐬
void EditScene::CreateModels(std::shared_ptr<FactoryManager> fm)
{
	m_grassModel = // ���u���b�N
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");

	m_coinModel = // �R�C���u���b�N
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");

	m_cloudModel = // �_�u���b�N
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");

	m_gravityModel = // �d�̓u���b�N
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	m_playerModel = // �v���C�����f��
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Bird.cmo");

	m_noneModel = // �����S�����f��
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Eraser.cmo");
}

// ���f���̐������Z�b�g����
void EditScene::ResetObjNum()
{
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_blockCount[m_mapObj[i].id] = 0;
	}
}

// ���f���̐����J�E���g����
void EditScene::CountObjNum()
{
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_blockCount[m_mapObj[i].id]++;
	}
}

// �A�C�R�����N���b�N����
void EditScene::ClickIcons()
{
	auto& _input = Input::GetInstance();
	auto& _sound = GetSystemManager()->GetSoundManager();

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

	// �v���C�V�[���ɍs��
	if (m_editUI->GetGoPlayFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		ChangeScene(SCENE::PLAY);
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

// �}�b�v�̕ҏW
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

	// �I���s��ۑ�
	m_XZCheck = m_cursorPos;

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

// �}�b�v�ǂݍ���
void EditScene::LoadMap(std::wstring filename)
{
	// �}�b�v�̓ǂݍ���
	m_mapLoader.LoadMap(filename);

	// �p�X���擾�ł��Ȃ���ΐV�������ʂ̃}�b�v���쐬����
	m_filePath = m_mapLoader.GetFilePath();

	// �}�b�v���i�[����
	m_mapObj = m_mapLoader.GetMapData();

	// ���f���̐��𐔂���
	ResetObjNum();
	CountObjNum();
}

// �t�@�C�������o��
void EditScene::SaveFile()
{
	m_mapLoader.WriteMap(m_mapObj);		// �t�@�C���̏����o��
}

// �v�f�`�F�b�N
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

// UndoRedo�����s
void EditScene::DoUndoRedo()
{
	auto& _input = Input::GetInstance();

	// �O�ɖ߂�
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Z))
	{
		RestoreHistory(m_history.GetUndo());
		ResetObjNum();
		CountObjNum();
	}
	// Undo��������
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::X))
	{
		RestoreHistory(m_history.GetRedo());
		ResetObjNum();
		CountObjNum();
	}
}

// UndoRedo�ۑ�
void EditScene::SaveModification()
{
	// ��Ԃ�ۑ�
	m_history.AddHistory(MementoMap(m_mapObj));
}

// UndoRedo�̓K�p
void EditScene::RestoreHistory(MementoMap mement)
{
	m_mapObj = mement.GetState();
}
