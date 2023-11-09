/*
 *	@File	GameMain.cpp
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "GameMain.h"
#include "Libraries/Nlohmann/json.hpp" // Json�t�@�C���̓��o�͂Ɏg�p

// JSON for Modern C++
//------------------------------------------------------------------------//
// Json�t�@�C���̃_�E�����[�h��						                      //
//																		  //
// JSON for Modern C++(nlohmann/json)���g�p								  //
// https://github.com/nlohmann/json										  //
//------------------------------------------------------------------------//

// �t�@�C�����o�͂Ɏg�p
#include <fstream>

// �t�F�[�h�I�u�W�F�N�g
#include "Libraries/SystemDatas/Fade.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

// TODO: �V�[���Q�F�V�[���̃C���N���[�h
#include "Game/TitleScene/TitleScene.h"
#include "Game/SelectScene/SelectScene.h"
#include "Game/EditScene/EditScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

// �R���X�g���N�^
GameMain::GameMain()
	: m_nextScene{ SCENE::TITLE }		// �ŏ��̃V�[��
	, m_prevScene{ SCENE::NONE }		// �O��̃V�[��
	, m_nowScene{ nullptr }				// ���̃V�[���|�C���^
	, m_num{1}							// �X�e�[�W�ԍ�
	, m_clearTime{0.0f}					// �N���A�^�C��
	, m_coinNum{0}						// �l���R�C����
	, m_screenWidth{}					// �X�N���[���T�C�Y(��)
	, m_screenHeight{}					// �X�N���[���T�C�Y(�c)
	//�ȉ��̓Z�[�u�f�[�^�Ή�------------------------------------------------------------------------//
	, m_safeNum{}						// ���J���X�e�[�W
	, m_allCoins{}						// �݌v�R�C������
	, is_saveOnce{false}				// �Z�[�u�ς݂��m�F����t���O
{
}

// �f�X�g���N�^
GameMain::~GameMain()
{
	Finalize();
}

// ����������
void GameMain::Initialize()
{
	// �V�[���쐬
	CreateScene();

	// �t�F�[�h�C��
	m_fade->SetFadeIn();

	// �X�e�[�W�ԍ�
	m_num = 1;

	is_saveOnce = false;

	// �Z�[�u�f�[�^��ǂݍ���
	LoadSaveData();
}

// �X�V����
void GameMain::Update()
{
	// �t�F�[�h�̍X�V
	m_fade->Update();

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextScene != SCENE::NONE && m_fade->GetEndFlag())
	{
		// �V�[���폜
		DeleteScene();

		// �V�[���쐬
		CreateScene();
	}

	// ���̂�����΍X�V
	if (m_nowScene != nullptr)
	{
		// �V�[���̍X�V����
		m_nowScene->Update();

		// �t�F�[�h�̒l���Z�b�g
		m_nowScene->SetFadeValue(m_fade->GetFadeNum());

		// �t�F�[�h���I����Ă�����V�[����؂�ւ���
		if (m_fade->GetEndFlag())
		{
			m_nextScene = m_nowScene->GetNextScene();
		}
		else // �t�F�[�h���I���܂ň������߂�
		{
			m_nowScene->StopNextScene();
		}
	}
}

// �`�揈��
void GameMain::Draw()
{
	// ���Ԃ�����Ε`��
	if (m_nowScene != nullptr)
	{
		m_nowScene->Draw();
	}

	// �t�F�[�h�̕`��
	m_fade->Draw();
}

// �I������
void GameMain::Finalize()
{
	m_nowScene.reset();
}

// �V�[���쐬
void GameMain::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_nowScene != nullptr) return;

	// TODO: �V�[���R�F�V�[���͂�������ǉ�
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// �^�C�g���V�[��
		{
			m_nowScene = std::make_unique<TitleScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::SELECT:		// �X�e�[�W�Z���N�g�V�[��
		{
			// ���v�R�C������999�ŏ���~��
			m_allCoins = UserUtility::Clamp(m_allCoins, 0, 999);

			m_nowScene = std::make_unique<SelectScene>(m_num, m_safeNum, m_allCoins);

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::PLAY:		// �Q�[���V�[��
		{
			m_nowScene = std::make_unique<PlayScene>(m_num, m_allCoins);

			m_fade->SetFadeSpeed(PLAY_FADE_SPEED);
			break;
		}
		case SCENE::RESULT:		// ���U���g�V�[��
		{
			m_nowScene = std::make_unique<ResultScene>(m_num, m_coinNum, m_clearTime);

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::EDIT:		// �X�e�[�W�G�f�B�^
		{
			m_nowScene = std::make_unique<EditScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::ENDGAME:	// �Q�[���I��
		{
			WriteSaveData(); // �f�[�^�������o��
			m_nowScene = std::make_unique<TitleScene>();
			m_nowScene->ExitApp();
			break;
		}
		default:
			break;
	}

	// �쐬�����V�[����������
	m_nowScene->SetScreenSize({ static_cast<float>(m_screenWidth) , static_cast<float>(m_screenHeight) });
	m_nowScene->Initialize();

	// �t�F�[�h�C��
	m_fade->SetFadeIn();

	// ���ւ̃V�[������������
	m_nextScene = SCENE::NONE;

	is_saveOnce = false;
}

// �V�[���폜
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (!m_nowScene) return;

	switch (m_nextScene)
	{
	case SCENE::EDIT:
		// �����p���������R�C����ێ�
		m_allCoins = CastSceneType<SelectScene>(m_nowScene)->GetAllCoins();
		break;
	case SCENE::PLAY:
		// �G�f�B�^�ō�����X�e�[�W���Z�b�g����
		if (m_prevScene == SCENE::EDIT)	{ m_num = 0; }
		// �ēǂݍ��݂łȂ���Ώ�������
		else if (m_prevScene != m_nextScene && m_prevScene != SCENE::RESULT)
		{
			// �X�e�[�W�ԍ���ێ�
			m_num = CastSceneType<SelectScene>(m_nowScene)->GetStageNum();
		}
		else if(m_prevScene != m_nextScene && m_prevScene == SCENE::RESULT)
		{
			m_num = CastSceneType<ResultScene>(m_nowScene)->GetStageNum();
		}
		break;
	case SCENE::RESULT:
		if (not CastSceneType<ResultScene>(m_nowScene))
		{
			// �N���A�^�C���Ɗl���R�C������ێ�
			m_clearTime = CastSceneType<PlayScene>(m_nowScene)->GetClearTime();
			m_coinNum = CastSceneType<PlayScene>(m_nowScene)->GetCoinNum();
			OpenNewStage();
		}
		break;
	default:
		break;
	}

	// ���V�[���̏I������
	if (m_fade->GetFadeNum() >= m_fade->GetMaxValue())
	{
		// ���V�[���̊��S�폜
		m_nowScene->Finalize();
		m_nowScene.reset();

		// �O�̃V�[����ۑ�
		m_prevScene = m_nextScene;
	}
	else // �t�F�[�h�A�E�g
	{
		m_fade->SetFadeOut();
	}
}

// �f�o�C�X�n�ݒ�
void GameMain::CreateWindowDependentResources(const int& screenWidth, const int& screenHeight)
{
	// �X�N���[���T�C�Y�̐ݒ�
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �t�F�[�h�I�u�W�F�N�g�̏�����
	m_fade = std::make_unique<Fade>(DEFAULT_FADE_SPEED);
	m_fade->Initialize();
}

// �Z�[�u�f�[�^�ǂݍ���
void GameMain::LoadSaveData()
{
	using Json = nlohmann::json;

	// JSON�f�[�^���t�@�C���ɓǂݍ���
	std::ifstream _file("Datas/Progress.json", std::ios::in);

	if (_file.is_open())
	{
		Json _input = {};
		_file >> _input;

		// �f�[�^���i�[����
		m_allCoins   = _input["CoinNum"];	// ���L�R�C����
		m_safeNum = _input["SafeStage"];	//���J���X�e�[�W��

		_file.close();
	}
}

// �Z�[�u�f�[�^�����o��
void GameMain::WriteSaveData()
{
	using Json = nlohmann::json;
	const int _JSON_INDENT = 4;	// �C���f���g

	// JSON�f�[�^���t�@�C���ɏ�������
	std::ofstream _file("Datas/Progress.json", std::ios::out);

	if (_file.is_open())
	{
		Json _output = {};

		// �f�[�^���Z�b�g
		_output =
		{
			 {"CoinNum", m_allCoins},		// ���L�R�C����
			 {"SafeStage",m_safeNum}		//���J���X�e�[�W��
		};

		// �C���f���g�𑵂��ď����o��
		_file << _output.dump(_JSON_INDENT);

		_file.close();
	}
}

// �X�e�[�W�������
void GameMain::OpenNewStage()
{
	if (m_nextScene != SCENE::RESULT || is_saveOnce) return;

	// �v���C���̃X�e�[�W�̃R�C���������ׂĊl��������J��
	if (CastSceneType<PlayScene>(m_nowScene)->GetCoinNum() ==
		CastSceneType<PlayScene>(m_nowScene)->GetMaxCoinCount())
	{
		// �S�ĉ�����Ă����珈�����Ȃ�
		m_safeNum -= m_safeNum > 0 ? 1 : 0;
	}

	// �R�C���������Z
	m_allCoins += m_coinNum;

	// �ŐV��Ԃ�ۑ�
	WriteSaveData();

	// ���ۑ�����Ȃ��悤�ɂ���
	is_saveOnce = true;
}