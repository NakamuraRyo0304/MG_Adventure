/*
 *	@File	ResultScene.cpp
 *	@Brief  ���U���g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // CSV�ǂݍ���
#include "Libraries/SystemDatas/MapLoad.h"

// �u���b�N
#include "../PlayScene/Objects/Blocks.h"

// UI
#include "Objects/ResultUI.h"

#include "ResultScene.h"

// �R���X�g���N�^
ResultScene::ResultScene()
	: IScene()					// ���N���X�̏�����
	, m_coinNum{}				// �R�C���̐�
	, m_clearTime{0.0f}			// �N���A�^�C�����i�[
	, m_saveTime{0.0f}			// �N���A�^�C����ۑ�����ϐ�
	, m_directionTime{0.0f}		// ���o���鎞��
	, m_stageNum{1}				// �w�i�̃X�e�[�W�ԍ�(��������1)
	, m_selectingScene{ 0 }		// ���ݑI�𒆂̃V�[��
{

}

// �f�X�g���N�^
ResultScene::~ResultScene()
{
}

// ����������
void ResultScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// �ϐ��̏�����
	SetSceneValues();

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_RESULT, true);
}

// �X�V����
void ResultScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �����̃A�j���[�V����
	AnimationValue();

	// �G�X�P�[�v�ŏI��
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME); }

	// ���j���[�Z���N�g
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Right) ||
	    _input.GetKeyTrack()->IsKeyReleased(Keyboard::D))
	{
		// �t�F�[�h���͏������Ȃ�
		if (GetFadeValue() >= 0.7f) return;

		m_selectingScene++;
		m_selectingScene = m_selectingScene ==  3 ? RETRY : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}
	else if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Left) ||
			 _input.GetKeyTrack()->IsKeyReleased(Keyboard::A))
	{
		// �t�F�[�h���͏������Ȃ�
		if (static_cast<int>(GetFadeValue()) != 0) return;

		m_selectingScene--;
		m_selectingScene = m_selectingScene == -1 ? TITLE : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}

	// UI�̍X�V
	m_resultUI->Update(static_cast<int>(m_clearTime));
	// ���ݑI�𒆂̃V�[�����Z�b�g
	m_resultUI->SetSelecting(m_selectingScene);
	// �l���R�C�������Z�b�g
	m_resultUI->SetCoins(m_coinNum);

	// Space�L�[�ŃV�[���؂�ւ�
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (static_cast<int>(GetFadeValue()) != 0) return;

		// �V�[������
		NextScene();

		// ���艹��炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
}

// �`��֐�
void ResultScene::Draw()
{
	// �`��֘A
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����p�s��
	SimpleMath::Matrix  _view, _projection;

	// �r���[�s��
	SimpleMath::Vector3    _eye(cosf(_timer), 20.0f + sinf(_timer) * 2.0f, 10.0f);
	SimpleMath::Vector3     _up(0.0f, 5.0f, 0.0f);
	SimpleMath::Vector3 _target(0.0f, -10.0f, -5.0f);

	_view = SimpleMath::Matrix::CreateLookAt(_eye, _target, _up);

	// �v���W�F�N�V�����s��
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	m_blocks->Render(_states, _view, _projection, _timer, SimpleMath::Vector3{ 1.0f,-1.0f,-1.0f });

	// UI�̕\��
	m_resultUI->Render(GetFadeValue());
}


// �I������
void ResultScene::Finalize()
{
	// �}�b�v�̌㏈��
	m_blocks->Finalize();

	// UI�̌㏈��
	m_resultUI->Finalize();
}

// ��ʁA�f�o�C�X�ˑ��̏�����
void ResultScene::CreateWindowDependentResources()
{
	// �V�X�e���̍쐬
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, 45.0f);

	// UI�̍쐬
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_resultUI = std::make_unique<ResultUI>();
	m_resultUI->Create(GetSystemManager(), GetScreenSize());

	// �u���b�N�̍쐬
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());

	// ���f���̍쐬
	GetFactoryManager()->BuildModelFactory();
	CreateModels(GetFactoryManager());
	GetFactoryManager()->LeaveModelFactory();
}

// �V�[���ϐ��������֐�
void ResultScene::SetSceneValues()
{
	// �^�C�}�[�̕ۑ�
	m_saveTime = m_clearTime;

	// ���o���� �ŏ��̃t�F�[�h���l�����đ��߂Ɏ��
	m_directionTime = 120.0f;

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);
}

// ���f���̍쐬
void ResultScene::CreateModels(std::shared_ptr<FactoryManager> fm)
{
	auto _grass = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
	auto _coin = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
	auto _cloud = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
	auto _gravity = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	// ���f���̎󂯓n��
	m_blocks->CreateModels(std::move(_grass), m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin), m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud), m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// �����̃A�j���[�V����
bool ResultScene::AnimationValue()
{
	// ���o���Ԃ��J�E���g
	m_directionTime--;

	// �����_���̒l�𐶐�
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 69);

	// �����_���Ȑ����𐶐�
	int randomValue = dist(gen);

	// ���o������
	if (m_directionTime < 0.0f)
	{
		m_directionTime = 0.0f;
		m_clearTime = MAX_TIME - m_saveTime;

		return true;
	}
	else
	{
		// �����_���Ȓl������
		m_clearTime = static_cast<float>(randomValue);
	}

	return false;
}

// �V�[���؂�ւ�
void ResultScene::NextScene()
{
	switch (m_selectingScene)
	{
	case RETRY:
		ChangeScene(SCENE::PLAY);
		break;
	case SELECT:
		ChangeScene(SCENE::SELECT);
		break;
	case TITLE:
		ChangeScene(SCENE::TITLE);
		break;
	default:
		break;
	}
}
