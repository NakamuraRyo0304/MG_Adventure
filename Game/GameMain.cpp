/*
 *	@File	GameMain.cpp
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

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
	m_nextScene{ SCENE::SELECT },
	m_prevScene{ SCENE::NONE },
	m_nowScene{ nullptr },
	m_num{1},
	m_clearTime{0.0f},
	m_screenWidth{},
	m_screenHeight{}
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

	// �G�X�P�[�v�ŃQ�[���I��
	if (kb.Escape)
	{
		m_nowScene->ExitApp();
	}

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
	// �V�[���폜
	DeleteScene();
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
			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::EDIT:		// �X�e�[�W�G�f�B�b�g�V�[��
		{
			m_nowScene = std::make_unique<EditScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
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

	// ���݂��Z���N�g�V�[���Ȃ�X�e�[�W�ԍ���ێ�
	if (m_nextScene == SCENE::PLAY)
	{
		// �ēǂݍ��݂łȂ���Ώ�������
		if (m_nextScene != m_prevScene && m_prevScene != SCENE::RESULT)
		{
			m_num = CastSceneType<SelectScene>(m_nowScene)->GetStageNum();
		}
	}

	// ���݂��v���C�V�[���Ȃ�^�C�����~�b�g��ێ�
	if (m_nextScene == SCENE::RESULT)
	{
		if (CastSceneType<ResultScene>(m_nowScene) == nullptr)
		{
			m_clearTime = CastSceneType<PlayScene>(m_nowScene)->GetTimeLimit();
		}
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