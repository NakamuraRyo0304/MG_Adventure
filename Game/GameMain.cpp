/*
 *	@File	GameMain.cpp
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

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
	m_nextScene(SCENE::TITLE),
	m_pNowScene(nullptr),
	m_num(1),
	m_screenWidth(),
	m_screenHeight()
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

	// �X�e�[�W�ԍ�
	m_num = 1;
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void GameMain::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	auto keyState = DirectX::Keyboard::Get().GetState();
	
	m_keyboardStateTracker->Update(keyState);

	// �}�E�X���͏����擾����
	auto mouseState = DirectX::Mouse::Get().GetState();

	m_mouseStateTracker->Update(mouseState);

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextScene != SCENE::NONE)
	{
		// �V�[���폜
		DeleteScene();
		
		// �V�[���쐬
		CreateScene();
	}

	// ���Ԃ�����΍X�V
	if (m_pNowScene != nullptr)
	{
		// �^�C�}�[�ƃL�[�{�[�h�̎󂯓n��
		m_pNowScene->Update(static_cast<float>(timer.GetElapsedSeconds()), keyState,mouseState);
				
		// NONE�ȊO���������珈��
		m_nextScene = m_pNowScene->GetNextScene();
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void GameMain::Draw()
{
	// ���Ԃ�����Ε`��
	if (m_pNowScene != nullptr)
	{
		m_pNowScene->Draw();
	}
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
	if (m_pNowScene != nullptr)
	{
		return;
	}	

	// TODO: �V�[���R�F�V�[���͂�������ǉ�
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// �^�C�g���V�[��
		{
			m_pNowScene = std::make_unique<TitleScene>();
			break;
		}
		case SCENE::SELECT:		// �X�e�[�W�Z���N�g�V�[��
		{
			m_pNowScene = std::make_unique<SelectScene>();
			break;
		}
		case SCENE::PLAY:		// �Q�[���V�[��
		{
			m_pNowScene = std::make_unique<PlayScene>();
			break;
		}
		case SCENE::RESULT:		// ���U���g�V�[��
		{
			m_pNowScene = std::make_unique<ResultScene>();
			break;
		}
		case SCENE::EDIT:		// �X�e�[�W�G�f�B�b�g�V�[��
		{
			m_pNowScene = std::make_unique<EditScene>();
			break;
		}
		default:
			break;
	}

	// �v���C�V�[���ɂȂ�����󂯓n��
	if (m_nextScene == SCENE::PLAY)
	{
		m_pNowScene->SetStageNum(m_num);
	}

	// �쐬�����V�[����������
	m_pNowScene->Initialize();

	// ���ւ̃V�[������������
	m_nextScene = SCENE::NONE;
}

//--------------------------------------------------------//
//�V�[���폜                                              //
//--------------------------------------------------------//
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_pNowScene == nullptr)
	{
		return;
	}

	// ���݂��Z���N�g�V�[���Ȃ�ێ�
	if (m_nextScene == SCENE::PLAY)
	{
		m_num = m_pNowScene->GetStageNum();
	}

	// ���V�[���̏I������
	m_pNowScene->Finalize();

	// ���V�[���̊��S�폜
	m_pNowScene.reset();
}

//--------------------------------------------------------//
//��ʃT�C�Y�ˑ��̏�����                                  //
//--------------------------------------------------------//
void GameMain::CreateWindowDependentResources(const int& screenWidth, const int& screenHeight)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �X�N���[���T�C�Y�̐ݒ�
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �L�[�{�[�h�֘A
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// �}�E�X�֘A
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouseStateTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(pDR->GetHwnd());
}
