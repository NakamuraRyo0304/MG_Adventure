/*
 *	@File	PlayScene.cpp
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/ColliderHelper.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Objects/Player.h"
#include "Objects/PlayerBill.h"
#include "Objects/PlayUI.h"
#include "Objects/PlaySky.h"
#include "System/PlayCamera.h"
#include "../CommonObjects/Blocks.h"
#include "PlayScene.h"

// �R���X�g���N�^
PlayScene::PlayScene(const int& stageNum, const int& coins)
	: IScene()						// ���N���X�̏�����
	, m_stageNum{ stageNum }		// �X�e�[�W�ԍ�
	, m_allCoins{ coins }			// �ۗL�R�C����
	, m_startTimer{}				// �J�n����
	, m_timeLinits{}				// ��������
	, m_clearTime{}					// �N���A����
	, m_fallValue{}					// �����p�ϐ�
	, m_lighting{}					// ���C�e�B���O�ݒ�
	, is_helpFlag{false}			// �w���v�\���t���O
{
}

// �f�X�g���N�^
PlayScene::~PlayScene()
{
	m_player.reset();
	m_blocks.reset();
	m_playerPoint.reset();
	m_playUI.reset();
	m_sky.reset();
	m_camera.reset();
	m_collider.reset();
}

// ����������
void PlayScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �ϐ��̏�����
	SetSceneValues();
}

// �X�V����
void PlayScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �G�X�P�[�v�ŃZ���N�g�ɖ߂�
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::SELECT);}

	// UI�\�����͈ȍ~�������Ȃ�
	if (UpdateUI()) return;

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �J�E���g�_�E�����I�������X�^�[�g
	if (Countdown() == false)
	{
		// �X�^�[�g�̓���
		FirstMovement();
		return;
	}

	// �J�������쐧��������
	GetSystemManager()->GetCamera()->SetEagleMode(true);
	GetSystemManager()->GetCamera()->SetArrowMode(true);

	// �R�C�������ׂĊl�����^�C���A�b�v�Ń��U���g
	if (m_blocks->IsCollectedFlag() || m_timeLinits < 0.0f)
	{
		// �N���A�^�C�����i�[
		m_clearTime = m_timeLinits / FLAME_RATE;

		ChangeScene(SCENE::RESULT);
		return;
	}
	else // �N���A���Ă��Ȃ���΃f�N�������g
	{
		// �������Ԃ̌v�Z
		m_timeLinits--;

		// ��̏���
		m_sky->Update(m_timeLinits / (TIME_LIMIT * FLAME_RATE));

		// ���Ԃ������ɂȂ����獇�}��炷
		if (static_cast<int>(m_timeLinits / FLAME_RATE) == TIME_LIMIT / 2)
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_EVENINGBELL, false);
		}
	}

	// �v���C���̍X�V
	m_player->Update();

	// �v���C���[�̈ʒu�Ƀ|�C���g�摜��\��
	m_playerPoint->SetVertexMovePos(m_player->GetPosition());

	// �X�e�[�W�̉��ɗ�������X�e�[�W���󉉏o
	if (m_player->GetPosition().y < DURATION_FLOOR_LINE)
	{
		// ����������
		m_fallValue -= FALL_SPEED;

		for (auto& i : m_blocks->GetMapData())
		{
			// �G�t�F�N�g���I�t
			m_playUI->SetEffectFlag(false);

			// �I�u�W�F�N�g�̐U��
			GetSystemManager()->GetCamera()->ShakeObject(
				SHAKE_DURATION,							// �U������
				SHAKE_TREMOR * SHAKE_OBJ_POWER,			// �U���͈�
				&m_blocks->GetBlockPosition(i.index)	// �U���I�u�W�F�N�g
			);

			// ���W�̃Z�b�g
			m_blocks->SetBlockPosition(
				SimpleMath::Vector3(
				i.position.x,						// X��
				i.position.y + m_fallValue,			// Y��
				i.position.z),						// Z��
				i.index								// �z��ԍ�
			);
		}
	}
	// ���������玀�S�G�t�F�N�g���o��
	else if (m_player->GetPosition().y < DEATH_EFFECT_LINE)
	{
		// �I�u�W�F�N�g�̐U��
		for (auto& i : m_blocks->GetMapData())
		{
			// �G�t�F�N�g���I��
			if (static_cast<int>(m_player->GetPosition().y) % 2 == 0)
			{
				m_playUI->SetEffectFlag(true);
			}
			else
			{
				m_playUI->SetEffectFlag(false);
			}
			GetSystemManager()->GetCamera()->ShakeObject(
				1.0f,									// �U������
				SHAKE_TREMOR,							// �U���͈�
				&m_blocks->GetBlockPosition(i.index)	// �U���I�u�W�F�N�g
			);
		}
	}
	// �v���C���[�������̂ݏ���
	else
	{
		// �u���b�N�̍X�V
		m_blocks->Update();

		// �����蔻��̍X�V
		m_collider->Update(m_player.get(),m_blocks.get());
	}

	// UI�̍X�V
	m_playUI->Update(m_timeLinits);

	// ���������烊�X�^�[�g
	if (m_player->GetDeathFlag())
	{
		ChangeScene(SCENE::PLAY);
	}
}

// �`�揈��
void PlayScene::Draw()
{
	// �`��֘A
	auto& _states = GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����p�s��
	SimpleMath::Matrix _view, _projection;

	// �r���[�s��&�v���W�F�N�V�����s��
	if (Countdown() == false)
	{
		// �X�^�[�g���o�J����
		_view = m_camera->CreateView();
		_projection = m_camera->GetProjection();
	}
	else
	{
		// �ʏ�J����
		_view = GetSystemManager()->GetCamera()->GetView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}

	// ���C�e�B���O�̍X�V
	m_blocks->SetLighting(m_lighting);

	// �}�b�v�̕`��
	m_blocks->Render(*_states, _view, _projection, _timer, m_lighting);

	// �v���C���[�̕`��
	m_player->Render(*_states, _view, _projection);

	// �X�J�C�h�[���̕`��
	m_sky->Draw(*_states, _view, _projection, _timer);

	// �r���{�[�h�̕`��
	auto& _position = GetSystemManager()->GetCamera()->GetPosition();
	auto& _target = GetSystemManager()->GetCamera()->GetTarget();
	m_playerPoint->CreateBillboard(_target, _position, SimpleMath::Vector3::Up);
	m_playerPoint->Render(m_player->GetPosition(), _timer, _view, _projection);

	// UI�̕`��
	m_playUI->IsCountDownEnd() ? m_playUI->Render() : m_playUI->RenderCountDown(m_startTimer);
}

// �I������
void PlayScene::Finalize()
{
	// �v���C���̌㏈��
	m_player->Finalize();

	// �}�b�v�̌㏈��
	m_blocks->Finalize();
}

// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
void PlayScene::CreateWindowDependentResources()
{
	// �V�X�e���̍쐬
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// �f�t�H���g�J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// �X�^�[�g�p�J�����̍쐬
	m_camera = std::make_unique<PlayCamera>(SimpleMath::Vector2(GetScreenSize().x, GetScreenSize().y));
	m_camera->SetPosition(START_CAMERA_POS);

	// �X�J�C�h�[���̍쐬
	m_sky = std::make_unique<PlaySky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// �v���C���[�̍쐬
	CreatePlayer();

	// �u���b�N�̍쐬
	CreateBlock();

	// �ʒu���̃V�F�[�_�[�̍쐬
	m_playerPoint = std::make_unique<PlayerBill>(GetFactoryManager());
	m_playerPoint->Create();

	// UI�̍쐬
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_playUI = std::make_unique<PlayUI>();
	m_playUI->Create(GetSystemManager(), GetScreenSize());

	// �����蔻��̍쐬
	m_collider = std::make_unique<ColliderHelper>(JUDGE_AREA, GetSystemManager());
}

// �V�[���ϐ��������֐�
void PlayScene::SetSceneValues()
{
	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	GetSystemManager()->GetCamera()->SetArrowMode(false);

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_PLAY, true);

	// �������Ԃ̏�����
	// ���ԁ@�~�@�t���[�����[�g
	m_timeLinits = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// �J�n�J�E���g�_�E��(�t�F�[�h���l��)
	m_startTimer = START_COUNT * FLAME_RATE;

	// ���S�G�t�F�N�g��؂�
	m_playUI->SetEffectFlag(false);

	// �w���v��\�����Ȃ�
	is_helpFlag = false;

	// �v���C���̏�����
	m_player->SetAllCoins(m_allCoins);
	m_player->Initialize();

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);

	// �v���C���[���W�ݒ�
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// ���C�e�B���O�ݒ�
	m_lighting = -SimpleMath::Vector3::One;
}

// �v���C���[�쐬�֐�
void PlayScene::CreatePlayer()
{
	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// �t�@�N�g���[���烂�f�������炤
	auto _head = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");

	_fm->LeaveModelFactory();

	// �v���C���[���쐬����
	m_player = std::make_unique<Player>(std::move(_head),
		std::move(_body), std::move(_legR), std::move(_legL));
}

// �u���b�N�쐬�֐�
void PlayScene::CreateBlock()
{
	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// �t�@�N�g���[���烂�f�������炤
	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineBox.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCoin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCl.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// �u���b�N���쐬����
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks->CreateShader();

	// ���f���̎󂯓n��
	m_blocks->CreateModels(std::move(_grass),   m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin),    m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud),   m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// �J�E���g�_�E��
bool PlayScene::Countdown()
{
	m_startTimer -= COUNT_SPEED;

	if (m_startTimer <= 0.0f)
	{
		m_startTimer = 0.0f;
		return true;
	}

	return false;
}

// �X�^�[�g�̓���
void PlayScene::FirstMovement()
{
	auto& _camera = GetSystemManager()->GetCamera();

	// �J�������W�̈ړ�
	m_camera->SetPosition(
		UserUtility::Lerp(m_camera->GetPosition(), _camera->GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }));

	// �J���������_�̈ړ�
	m_camera->SetTarget(
		UserUtility::Lerp(m_camera->GetTarget(), _camera->GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }));
}

// UI�̍X�V
bool PlayScene::UpdateUI()
{
	auto& _input = Input::GetInstance();
	auto& _se = GetSystemManager()->GetSoundManager();

	// �w���v�\����؂�ւ���
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::H) && Countdown())
	{
		is_helpFlag = not is_helpFlag;
		m_playUI->SetHelpFlag(is_helpFlag);
		_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	if (is_helpFlag)
	{
		bool _leftFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left));
		bool _rightFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right));
		bool _upFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Up));
		bool _downFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Down));

		// �\���ؑ�
		m_playUI->UpdatePage(_leftFlag, _rightFlag);

		// �y�[�W���߂��鉹��炷
		if (_leftFlag || _rightFlag)
		{
			_se->PlaySound(XACT_WAVEBANK_SKBX_SE_NEXTHELP, false);
		}

		// �J�ڃy�[�W�̏ꍇ�AUpdateTransition��UpDownWS��n��
		if (m_playUI->GetTransitionPage())
		{
			// �㉺�L�[�őI��
			m_playUI->UpdateTransition(_upFlag, _downFlag);

			if (_upFlag || _downFlag)
			{
				_se->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
			}

			// ���肷��
			if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Space))
			{
				// �V�[���̑J��
				HelpNext();
				_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
			}
		}
	}

	// �w���v�\�����͑��̏��������Ȃ�
	return is_helpFlag;
}

// �w���v����V�[���J��
void PlayScene::HelpNext()
{
	using NextScene = PlayUI::GO;
	switch (m_playUI->GetTransNum())
	{
	case NextScene::RETRY:
		ChangeScene(SCENE::PLAY);
		break;
	case NextScene::SELECT:
		ChangeScene(SCENE::SELECT);
		break;
	case NextScene::TITLE:
		ChangeScene(SCENE::TITLE);
		break;
	case NextScene::EXIT:
		ChangeScene(SCENE::ENDGAME);
		break;
	default:
		break;
	}
}

// �l���R�C����
const int& PlayScene::GetCoinNum() { return m_blocks->GetCoinCount(); }

// �X�e�[�W�����v�R�C����
const int& PlayScene::GetMaxCoinCount() { return m_blocks->GetMaxCoinCount(); }
