/*
 *	@File	GameMain.cpp
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

 // �t�@�C���̕ۑ��Ɏg�p
#include<sstream>
#include<fstream>
#include<regex>

// �t�F�[�h�I�u�W�F�N�g
#include "../Libraries/SystemDatas/Fade.h"

// TODO: �V�[���Q�F�V�[���̃C���N���[�h
#include "Game/TitleScene/TitleScene.h"
#include "Game/SelectScene/SelectScene.h"
#include "Game/EditScene/EditScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
GameMain::GameMain():
	m_nextScene{ SCENE::TITLE },		// �ŏ��̃V�[��
	m_prevScene{ SCENE::NONE },			// �O��̃V�[��
	m_nowScene{ nullptr },				// ���̃V�[���|�C���^
	m_num{1},							// �X�e�[�W�ԍ�
	m_clearTime{0.0f},					// �N���A�^�C��
	m_coinNum{0},						// �l���R�C����
	m_screenWidth{},					// �X�N���[���T�C�Y(��)
	m_screenHeight{},					// �X�N���[���T�C�Y(�c)
	is_saveOnce{false},
	//�ȉ��̓Z�[�u�f�[�^�Ή�------------------------------------------------------------------------//
	m_closeNum{},						// ���J���X�e�[�W
	m_totalCoinNum{},					// �݌v�R�C������
	m_clearJudge{0}						// �N���A����
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
GameMain::~GameMain()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::Initialize()
{
	// �V�[���쐬
	CreateScene();

	// �t�F�[�h�C��
	m_fade->SetFadeIn();

	// �X�e�[�W�ԍ�
	m_num = 1;

	is_saveOnce = false;

	for (int i = 0; i < 10; ++i)
	{
		m_clearJudge[i] = 0;
	}

	// �Z�[�u�f�[�^��ǂݍ���
	LoadSaveData();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">���Ԃ��X�V����^�C�}�[</param>
/// <returns>�Ȃ�</returns>
void GameMain::Update(const DX::StepTimer& timer)
{
	// �t�F�[�h�̍X�V
	m_fade->Update();

	// ���Ԃ��X�V����
	float time = static_cast<float>(timer.GetTotalSeconds());

	// �L�[���͏����擾����
	auto kb = Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(kb);

	// �}�E�X���͏����擾����
	auto ms = Mouse::Get().GetState();
	m_mouseStateTracker->Update(ms);

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
		m_nowScene->Update(time, kb, ms);

		// �t�F�[�h���I����Ă�����V�[����؂�ւ���
		if (m_fade->GetEndFlag())
		{
			m_nextScene = m_nowScene->GetNextScene();
		}
		else
		{
			m_nowScene->StopNextScene();
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::Finalize()
{
}

/// <summary>
/// �V�[���̐V�K�쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
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
			m_nowScene = std::make_unique<SelectScene>();
			CastSceneType<SelectScene>(m_nowScene)->SetNoStageNum(m_closeNum);
			CastSceneType<SelectScene>(m_nowScene)->SetTotalCoins(m_totalCoinNum);

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::PLAY:		// �Q�[���V�[��
		{
			m_nowScene = std::make_unique<PlayScene>();

			// �I�������X�e�[�W��n��
			CastSceneType<PlayScene>(m_nowScene)->SetStageNum(m_num);
			m_fade->SetFadeSpeed(PLAY_FADE_SPEED);
			break;
		}
		case SCENE::RESULT:		// ���U���g�V�[��
		{
			m_nowScene = std::make_unique<ResultScene>();

			// �N���A�^�C����n��
			CastSceneType<ResultScene>(m_nowScene)->SetStageNum(m_num);
			CastSceneType<ResultScene>(m_nowScene)->SetClearTime(m_clearTime);
			CastSceneType<ResultScene>(m_nowScene)->SetCoinNum(m_coinNum);
			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::EDIT:		// �X�e�[�W�G�f�B�b�g�V�[��
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
	m_nowScene->Initialize();

	// �t�F�[�h�C��
	m_fade->SetFadeIn();

	// ���ւ̃V�[������������
	m_nextScene = SCENE::NONE;

	is_saveOnce = false;
}
/// <summary>
/// �V�[���̍폜
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (!m_nowScene) return;

	switch (m_nextScene)
	{
	case SCENE::PLAY:
		// �ēǂݍ��݂łȂ���Ώ�������
		if (m_nextScene != m_prevScene && m_prevScene != SCENE::RESULT)
		{
			// �X�e�[�W�ԍ���ێ�
			m_num = CastSceneType<SelectScene>(m_nowScene)->GetStageNum();
		}
		break;
	case SCENE::RESULT:
		if (CastSceneType<ResultScene>(m_nowScene) == nullptr)
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

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="screenWidth">��ʉ���</param>
/// <param name="screenHeight">��ʏc��</param>
/// <returns>�Ȃ�</returns>
void GameMain::CreateWindowDependentResources(const int& screenWidth, const int& screenHeight)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// �X�N���[���T�C�Y�̐ݒ�
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �L�[�{�[�h�֘A
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// �}�E�X�֘A
	m_mouse = std::make_unique<Mouse>();
	m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(pDR->GetHwnd());

	// �t�F�[�h�I�u�W�F�N�g�̏�����
	m_fade = std::make_unique<Fade>(DEFAULT_FADE_SPEED);
	m_fade->Initialize(context, device);
}

/// <summary>
/// �Z�[�u�f�[�^��ǂݍ���
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::LoadSaveData()
{
	//-------------------------------------------------------------------------------------//
	// ���J���X�e�[�W���Ɗl���R�C�����̎擾
	std::ifstream coinIfs(L"Resources/SaveData.txt");

	std::string line;

	// �f�[�^���Ȃ��Ȃ�܂Ŋi�[
	for (; std::getline(coinIfs, line); )
	{
		if (!coinIfs) return;

		// �J���}���󔒂ɕύX
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// �󔒂ŕ�������
		std::istringstream iss(tmp);

		if (!(iss >> m_closeNum))
		{
			coinIfs.close();
			return;
		}

		if (!(iss >> m_totalCoinNum))
		{
			coinIfs.close();
			return;
		}
	}
	coinIfs.close();

	//-------------------------------------------------------------------------------------//
	// �X�e�[�W�N���A���̎擾
	std::ifstream clearIfs(L"Resources/Restriction.txt");

	// �f�[�^���Ȃ��Ȃ�܂Ŋi�[
	for (int i = 0; std::getline(clearIfs, line); ++i)
	{
		if (!clearIfs) return;

		// �J���}���󔒂ɕύX
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// �󔒂ŕ�������
		std::istringstream clearIss(tmp);

		// 1���N���A,0�����N���A
		if (!(clearIss >> m_clearJudge[i]))
		{
			clearIfs.close();
			return;
		}
	}
	clearIfs.close();
}

/// <summary>
/// �Z�[�u�f�[�^�������o��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::WriteSaveData()
{
	//-------------------------------------------------------------------------------------//
	// ���J���X�e�[�W���Ɗl���R�C�����̏����o��
	std::ofstream coinOfs(L"Resources/SaveData.txt");

	// �t�@�C�����Ȃ���Ώ������Ȃ�
	if (coinOfs)
	{
		// �t�@�C�����o�͂���
		std::ostringstream coinOss;

		coinOss << m_closeNum << "," << m_totalCoinNum << ",";
		coinOfs << coinOss.str();

		coinOfs.close();
	}

	//-------------------------------------------------------------------------------------//
	// �N���A�󋵂������o��
	std::ofstream clearOfs(L"Resources/Restriction.txt");

	// �t�@�C�����Ȃ���Ώ������Ȃ�
	if (clearOfs)
	{
		// �t�@�C�����o�͂���
		std::ostringstream clearOss;

		for (int i = 0; i < 10; ++i)
		{
			clearOss << m_clearJudge[i] << ",";
		}
		clearOfs << clearOss.str();

		clearOfs.close();
	}
}

/// <summary>
/// �X�e�[�W�������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void GameMain::OpenNewStage()
{
	if (m_nextScene != SCENE::RESULT || is_saveOnce) return;

	// �v���C���̃X�e�[�W�̃R�C���������ׂĊl��������J��
	if (CastSceneType<PlayScene>(m_nowScene)->GetCoinNum() == CastSceneType<PlayScene>(m_nowScene)->GetMaxCoinCount() &&
		m_clearJudge[CastSceneType<PlayScene>(m_nowScene)->GetStageNum()] == 0)
	{
		// �S�ĉ�����Ă����珈�����Ȃ�
		m_closeNum -= m_closeNum > 0 ? 1 : 0;

		m_clearJudge[CastSceneType<PlayScene>(m_nowScene)->GetStageNum()] = 1;
	}

	m_totalCoinNum += m_coinNum;


	// �ŐV��Ԃ�ۑ�
	WriteSaveData();

	// ���ۑ�����Ȃ��悤�ɂ���
	is_saveOnce = true;
}
