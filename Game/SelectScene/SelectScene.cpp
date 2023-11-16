/*
 *	@File	SelectScene.cpp
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "System/SelectCamera.h"
#include "Objects/SelectUI.h"
#include "Objects/FontObject.h"
#include "Objects/SelectSky.h"
#include "../CommonObjects/Blocks.h"
#include "SelectScene.h"

// �R���X�g���N�^
SelectScene::SelectScene(const int& stageNum, const int& safeNum, const int& coins)
	: IScene()					// ���N���X�̏�����
	, m_stageNum{ stageNum }	// �X�e�[�W�ԍ�
	, m_safeNum{ safeNum }		// ���J���X�e�[�W�ԍ�
	, m_allCoins{ coins }		// ���v�R�C����
	, m_useCoins{}				// �g�p�R�C����
	, m_initCoins{}				// �J�n�R�C����
	, m_targetY{}				// �J�����̃^�[�Q�b�g��Y���W
	, is_selectEdit{false}		// �X�e�[�W�G�f�B�b�g��I��
{
}

// �f�X�g���N�^
SelectScene::~SelectScene()
{
	Finalize();
}

// ����������
void SelectScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �ϐ��̏�����
	SetSceneValues();
}

// �X�V����
void SelectScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();
	auto _key = Keyboard::Get().GetState();

	// �Z���N�g�J�����̍X�V
	m_selectCamera->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �X�e�[�W�̕ύX
	ChangeStageNumber();

	// UI�̍X�V
	m_selectUI->Update((_key.D || _key.Right), (_key.A || _key.Left));

	// �G�X�P�[�v�Ń^�C�g���ɖ߂�
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::TITLE); }

	// �R�C���̐���ۑ�
	m_allCoins = m_initCoins - static_cast<int>(m_useCoins);

	// Space�L�[�ŃV�[���؂�ւ�
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (GetFadeValue() >= 0.5f) return;

		// �X�e�[�W�ԍ���0�Ȃ�G�f�B�^�ɁA����ȊO�̓v���C��
		if (m_stageNum == 0)
		{
			is_selectEdit = m_allCoins >= STAGE_CREATE_PRICE;

			// ���b�Z�[�W
			if(m_allCoins < STAGE_CREATE_PRICE)
			{
				MessageBox(NULL,
					TEXT("�R�C����10���ȏ�擾���Ă��Ȃ����߁A���̋@�\�͎g�p�ł��܂���B"),
					TEXT("�R�C���s��"), MB_OK);
			}
		}
		else
		{
			ChangeScene(SCENE::PLAY);
		}

		// ���艹��炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	// �R�C�����o
	if (is_selectEdit)
	{
		// �R�C�������Z�b�g����
		m_selectUI->SetAllCoins(m_initCoins - static_cast<int>(m_useCoins));
		m_selectUI->MoveCoins(m_useCoins);

		// �g�p�����x��������J�ڂ���
		if (static_cast<int>(m_useCoins) == STAGE_CREATE_PRICE) { ChangeScene(SCENE::EDIT);	}
		else { m_useCoins += COUNT_SPEED; }
	}
}

// �`�揈��
void SelectScene::Draw()
{
	// �`��֘A
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����p�s��
	SimpleMath::Matrix _view, _proj;

	// �r���[�s��
	_view = m_selectCamera->GetView();

	// �v���W�F�N�V�����s��
	_proj = m_selectCamera->GetProjection();

	// �}�b�v�̕`��
	m_blocks[m_stageNum] != nullptr ? // �쐬�ς݂Ȃ�`�悷��
		m_blocks[m_stageNum]->Render(_states, _view, _proj, _timer,
			SimpleMath::Vector3{ 1.0f,-1.0f,-1.0f }) : void();

	// �X�J�C�h�[���̕`��
	m_skyDome->Draw(_states, _view, _proj, _timer);

	// �����̕`��
	m_fontObject->Render(_states, m_stageNum, _timer * 0.5f, _view, _proj);

	// UI�̕`��
	m_selectUI->Render(GetFadeValue(), m_stageNum, MAX_STAGE_NUM - 1);
}

// �I������
void SelectScene::Finalize()
{
	// ���f���폜
	for (size_t i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
	}
	m_selectUI->Finalize();
}

// ��ʁA�f�o�C�X�ˑ��̏�����
void SelectScene::CreateWindowDependentResources()
{
	// �V�X�e���̍쐬
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// �J�����̐ݒ�
	m_selectCamera = std::make_unique<SelectCamera>(GetScreenSize());

	// UI�̍쐬
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_selectUI = std::make_unique<SelectUI>();
	m_selectUI->Create(GetSystemManager(),GetScreenSize());

	// �t�H���g�I�u�W�F�N�g�̍쐬
	m_fontObject = std::make_unique<FontObject>(GetFactoryManager(),m_safeNum, MAX_STAGE_NUM);

	// �X�J�C�h�[�����f�����쐬����
	m_skyDome = std::make_unique<SelectSky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// ��ɕ`��Ώۂ��쐬���A���𗠂ŏ���
	CreateFirstStage();
	m_loadTask = std::async(std::launch::async, [&]() { CreateStages(); });
}

// �V�[���ϐ��������֐�
void SelectScene::SetSceneValues()
{
	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);

	// �R�C�������Z�b�g
	m_selectUI->SetAllCoins(m_allCoins);

	// �g�p�R�C���������Z�b�g(float�Ȃ̂̓R�C�������鉉�o�p)
	m_useCoins = 0.0f;

	// �J�n�R�C������ۑ�
	m_initCoins = m_allCoins;

	// �X�e�[�W�G�f�B�b�g�t���O��������
	is_selectEdit = false;
}

// �X�e�[�W�̃��[�h
void SelectScene::CreateStages()
{
	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		// �t�@�N�g���[���烂�f�������炤
		auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
		auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
		auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
		auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

		// �쐬����Ă��Ȃ��ꍇ�͍쐬����
		if (not m_blocks[i])
		{
			// �u���b�N�̍쐬
			m_blocks[i] = std::make_unique<Blocks>(GetFactoryManager());
			m_blocks[i]->CreateShader();

			// ���f���̎󂯓n��
			m_blocks[i]->CreateModels(std::move(_grass),   m_blocks[i]->GRASS);
			m_blocks[i]->CreateModels(std::move(_coin),    m_blocks[i]->COIN);
			m_blocks[i]->CreateModels(std::move(_cloud),   m_blocks[i]->CLOWD);
			m_blocks[i]->CreateModels(std::move(_gravity), m_blocks[i]->GRAVITY);

			// ����������
			m_blocks[i]->Initialize(i);
		}
	}

	_fm->LeaveModelFactory();
}
void SelectScene::CreateFirstStage()
{
	// �u���b�N�̍쐬
	m_blocks[m_stageNum] = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks[m_stageNum]->CreateShader();

	// �t�@�N�g���}�l�[�W��
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// ���f���̎󂯓n��
	m_blocks[m_stageNum]->CreateModels(std::move(_grass),   m_blocks[m_stageNum]->GRASS);
	m_blocks[m_stageNum]->CreateModels(std::move(_coin),    m_blocks[m_stageNum]->COIN);
	m_blocks[m_stageNum]->CreateModels(std::move(_cloud),   m_blocks[m_stageNum]->CLOWD);
	m_blocks[m_stageNum]->CreateModels(std::move(_gravity), m_blocks[m_stageNum]->GRAVITY);

	// ����������
	m_blocks[m_stageNum]->Initialize(m_stageNum);
}

// �X�e�[�W�I��
void SelectScene::ChangeStageNumber()
{
	// �؂�ւ��\�ȃ^�C�~���O�͂����ŕύX
	if (m_selectCamera->IsCanChange()) return;

	// �C���v�b�g�̍X�V
	auto _input = Keyboard::Get().GetState();

	if (_input.Right || _input.D)
	{
		// �X�e�[�W�ԍ����ő�Ȃ珈�����Ȃ�
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_safeNum) return;

		// �^�[�Q�b�g�̍X�V
		m_selectCamera->MoveTarget();

		// �I������炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_stageNum++;
	}
	if (_input.Left || _input.A)
	{
		// �X�e�[�W�ԍ���0�Ȃ珈�����Ȃ�
		if (m_stageNum == 0) return;

		// �^�[�Q�b�g�̍X�V
		m_selectCamera->MoveTarget();

		// �I������炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_stageNum--;
	}
}