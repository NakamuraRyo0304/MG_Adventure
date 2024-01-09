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
#include "System/ThirdPersonCamera.h"
#include "../CommonObjects/Blocks.h"
#include "PlayScene.h"

// �R���X�g���N�^
PlayScene::PlayScene(const int& stageNum, const int& coins)
	: IScene()						// ���N���X�̏�����
	, m_stageNum{ stageNum }		// �X�e�[�W�ԍ�
	, m_allCoins{ coins }			// �ۗL�R�C����
	, m_startTimer{0.0f}			// �J�n����
	, m_gameTimer{0.0f}				// ��������
	, m_clearTime{0.0f}				// �N���A����
	, m_fallValue{0.0f}				// �����p�ϐ�
	, m_lighting{}					// ���C�e�B���O�ݒ�
	, is_thirdPersonMode{false}		// �T�[�h�p�[�\�����[�h
	, is_helpFlag{false}			// �w���v�\���t���O
{
}

// �f�X�g���N�^
PlayScene::~PlayScene()
{
	m_player.reset();
	m_blocks.reset();
	m_playerBill.reset();
	m_playUI.reset();
	m_thirdCamera.reset();
	m_playSky.reset();
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
	if (is_thirdPersonMode) // �O�l�̃J����
	{
		m_thirdCamera->UpdateFollow(m_player->GetPosition(), m_player->GetNeckRotate(), THIRD_DISTANCE);
	}
	else // �����낵�J����
	{
		GetSystemManager()->GetCamera()->Update();
	}

	// �J�E���g�_�E�����I�������X�^�[�g
	if (StartTimer() == false)
	{
		// �X�^�[�g�̓���
		MoveStart();
		return;
	}

	// �J�������쐧��������
	GetSystemManager()->GetCamera()->SetEagleMode(true);
	GetSystemManager()->GetCamera()->SetArrowMode(true);

	// �T�[�h�p�[�\�����[�h�̐؂�ւ�
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Space))
	{
		is_thirdPersonMode = not is_thirdPersonMode;
	}

	// �R�C�������ׂĊl�����^�C���A�b�v�Ń��U���g
	if (m_blocks->IsCollectedFlag() || m_gameTimer < 0.0f)
	{
		// �N���A�^�C�����i�[
		m_clearTime = m_gameTimer / FLAME_RATE;

		ChangeScene(SCENE::RESULT);
		return;
	}
	else // �N���A���Ă��Ȃ���΃f�N�������g
	{
		// �������Ԃ̌v�Z
		m_gameTimer--;

		// ��̏���
		m_playSky->Update(m_gameTimer / (TIME_LIMIT * FLAME_RATE));

		// ���Ԃ������ɂȂ����獇�}��炷
		if (static_cast<int>(m_gameTimer / FLAME_RATE) == TIME_LIMIT / 2)
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_EVENINGBELL, false);
		}
	}

	// �v���C���̍X�V
	m_player->Update(is_thirdPersonMode);

	// ���Έړ�
	m_playerBill->SetVertexMovePos(m_player->GetPosition());

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
	m_playUI->Update(m_gameTimer);

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
	if (StartTimer() == false)
	{
		// �X�^�[�g���o�J����
		_view = m_camera->CreateView();
		_projection = m_camera->GetProjection();
	}
	else if (is_thirdPersonMode)
	{
		// ���A�����_(�O�l��)�J����
		_view = m_thirdCamera->GetFollowView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}
	else
	{
		// �ʏ�J����
		_view = GetSystemManager()->GetCamera()->GetView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}

	// ���C�e�B���O�̍X�V
	InitializeLighting();

	// �}�b�v�̕`��
	m_blocks->Render(*_states, _view, _projection, _timer, m_lighting);

	// �v���C���̕`��
	m_player->Render(*_states, _view, _projection, m_lighting);

	// �X�J�C�h�[���̕`��
	m_playSky->Draw(*_states, _view, _projection, _timer);

	// �r���{�[�h�̕`��
	if (not is_thirdPersonMode)
	{
		m_playerBill->CreateBillboard(
			GetSystemManager()->GetCamera()->GetTarget(),	// �J�����̒����_
			GetSystemManager()->GetCamera()->GetPosition(),	// �J�����̍��W
			SimpleMath::Vector3::Up
		);
		m_playerBill->Render(m_player->GetPosition(), _timer, _view, _projection);
	}
	// ������
	else
	{
		m_thirdCamera->DrawAdhesion();
	}

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

	// �T�[�h�p�[�\���J�����̍쐬
	m_thirdCamera = std::make_unique<ThirdPersonCamera>(GetSystemManager());
	m_thirdCamera->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// �X�^�[�g�p�J�����̍쐬
	m_camera = std::make_unique<PlayCamera>(SimpleMath::Vector2(GetScreenSize().x, GetScreenSize().y));
	m_camera->SetPosition(START_CAMERA_POS);

	// �X�J�C�h�[���̍쐬
	m_playSky = std::make_unique<PlaySky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// �v���C���[�̍쐬
	MakePlayer();

	// �u���b�N�̍쐬
	MakeBlocks();

	// �ʒu���̃V�F�[�_�[�̍쐬
	m_playerBill = std::make_unique<PlayerBill>(GetFactoryManager());
	m_playerBill->Create();

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
	m_gameTimer = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// �J�n�J�E���g�_�E��(�t�F�[�h���l��)
	m_startTimer = START_COUNT * FLAME_RATE;

	// ���S�G�t�F�N�g��؂�
	m_playUI->SetEffectFlag(false);

	// �T�[�h�p�[�\�����[�h��؂�
	is_thirdPersonMode = false;

	// �w���v��\�����Ȃ�
	is_helpFlag = false;

	// �v���C���̏�����
	m_player->SetAllCoins(m_allCoins);
	m_player->Initialize(GetSystemManager());

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);

	// �v���C���[���W�ݒ�
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// ���C�e�B���O�ݒ�
	m_lighting = -SimpleMath::Vector3::One;
}

// ���C�e�B���O�̐ݒ�
void PlayScene::InitializeLighting()
{
	m_player->InitializeLighting(m_lighting);
	m_blocks->InitializeLighting(m_lighting);
}

// �v���C���[�쐬�֐�
void PlayScene::MakePlayer()
{
	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// �t�@�N�g���[���烂�f�������炤
	auto _head = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");
	auto _wink = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head_w.cmo");

	_fm->LeaveModelFactory();
	// �v���C���[���쐬����
	m_player = std::make_unique<Player>(
		std::move(_head),			// ���̃��f��
		std::move(_body),			// �g�̂̃��f��
		std::move(_legR),			// �E���̃��f��
		std::move(_legL),			// �����̃��f��
		std::move(_wink)			// �E�C���N����
	);
}

// �u���b�N�쐬�֐�
void PlayScene::MakeBlocks()
{
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks->CreateShader();

	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// �t�@�N�g���[���烂�f�������炤
	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// ���f���̎󂯓n��
	m_blocks->CreateModels(std::move(_grass),   m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin),    m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud),   m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// �J�E���g�_�E��
bool PlayScene::StartTimer()
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
void PlayScene::MoveStart()
{
	auto& _camera = GetSystemManager()->GetCamera();

	//-------------------//
	// ���݈ʒu
	// �I�_�ʒu
	// �ړ����x
	//-------------------//

	// �J�������W�̈ړ�
	m_camera->SetPosition(
		UserUtility::Lerp(
			m_camera->GetPosition(),
			_camera->GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
	m_camera->SetTarget(
		UserUtility::Lerp(
			m_camera->GetTarget(),
			_camera->GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
}

// UI�̍X�V
bool PlayScene::UpdateUI()
{
	auto& _input = Input::GetInstance();
	auto& _se = GetSystemManager()->GetSoundManager();

	// �w���v�\����؂�ւ���
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Enter) && StartTimer())
	{
		is_helpFlag = not is_helpFlag;
		m_playUI->SetHelpFlag(is_helpFlag);
		_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	if (is_helpFlag)
	{
		// �\���ؑ�
		m_playUI->UpdatePage(
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left)),
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right)));

		// �y�[�W���߂��鉹��炷
		if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left) ||
			_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right))
		{
			_se->PlaySound(XACT_WAVEBANK_SKBX_SE_NEXTHELP, false);
		}

		// �J�ڃy�[�W�̏ꍇ�AUpdateTransition��UpDownWS��n��
		if (m_playUI->GetTransitionPage())
		{
			// �㉺�L�[�őI��
			m_playUI->UpdateTransition(
				(_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) ||
				 _input.GetKeyTrack()->IsKeyPressed(Keyboard::Up)),
				(_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) ||
				 _input.GetKeyTrack()->IsKeyPressed(Keyboard::Down)));

			if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) ||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::Up)||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) ||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::Down))
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
