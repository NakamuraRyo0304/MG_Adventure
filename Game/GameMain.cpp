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

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
GameMain::GameMain():
	m_nextScene{ SCENE::TITLE },
	m_nowScene{ nullptr },
	m_num{1},
	m_screenWidth{},
	m_screenHeight{},
	is_fadeOutFlag{}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
GameMain::~GameMain()
{
	Finalize();
}

//--------------------------------------------------------//
//������                                                  //
//--------------------------------------------------------//
void GameMain::Initialize()
{
	// �V�[���쐬
	CreateScene();

	// �t�F�[�h�C��
	m_fade->SetFadeIn();
	is_fadeOutFlag = false;

	// �X�e�[�W�ԍ�
	m_num = 1;
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void GameMain::Update(const DX::StepTimer& timer)
{
	// �t�F�[�h�̍X�V
	m_fade->Update(FADE_SPEED);

	// ���Ԃ��X�V����
	float time = static_cast<float>(timer.GetTotalSeconds());

	// �L�[���͏����擾����
	auto keyState = Keyboard::Get().GetState();

	m_keyboardStateTracker->Update(keyState);

	// �}�E�X���͏����擾����
	auto mouseState = Mouse::Get().GetState();

	m_mouseStateTracker->Update(mouseState);

	// �G�X�P�[�v�ŃQ�[���I��
	if (keyState.Escape)
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
		m_nowScene->Update(time, keyState, mouseState);

		// NONE�ȊO���������珈��
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

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void GameMain::Finalize()
{
	// �V�[���폜
	DeleteScene();
}

//--------------------------------------------------------//
//�V�[���쐬                                              //
//--------------------------------------------------------//
void GameMain::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_nowScene != nullptr)
	{
		return;
	}	
	
	// TODO: �V�[���R�F�V�[���͂�������ǉ�
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// �^�C�g���V�[��
		{
			m_nowScene = std::make_unique<TitleScene>();
			break;
		}
		case SCENE::SELECT:		// �X�e�[�W�Z���N�g�V�[��
		{
			m_nowScene = std::make_unique<SelectScene>();
			break;
		}
		case SCENE::PLAY:		// �Q�[���V�[��
		{
			m_nowScene = std::make_unique<PlayScene>();
			m_nowScene->SetStageNum(m_num);
			break;
		}
		case SCENE::RESULT:		// ���U���g�V�[��
		{
			m_nowScene = std::make_unique<ResultScene>();
			break;
		}
		case SCENE::EDIT:		// �X�e�[�W�G�f�B�b�g�V�[��
		{
			m_nowScene = std::make_unique<EditScene>();
			break;
		}
		default:
			break;	
	}

	// �쐬�����V�[����������
	m_nowScene->Initialize();

	// �t�F�[�h�C��
	m_fade->Reset();
	m_fade->SetFadeIn();

	// ���ւ̃V�[������������
	m_nextScene = SCENE::NONE;
}

//--------------------------------------------------------//
//�V�[���폜                                              //
//--------------------------------------------------------//
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (!m_nowScene) return;

	// ���݂��Z���N�g�V�[���Ȃ�ێ�
	if (m_nextScene == SCENE::PLAY)
	{
		m_num = m_nowScene->GetStageNum();
	}

	// ���V�[���̏I������
	if (m_fade->GetFadeNum() >= m_fade->GetMaxValue())
	{
		// ���V�[���̊��S�폜
		m_nowScene->Finalize();
		m_nowScene.reset();
	}
	else // �t�F�[�h�A�E�g
	{		
		m_fade->Reset();
		m_fade->SetFadeOut();
	}
}

//--------------------------------------------------------//
//��ʃT�C�Y�ˑ��̏�����                                  //
//--------------------------------------------------------//
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
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(context, device);
}