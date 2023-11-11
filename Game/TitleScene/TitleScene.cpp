/*
 *	@File	TitleScene.cpp
 *	@Brief	�^�C�g���V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Objects/Logo.h"
#include "Objects/Stage.h"
#include "Objects/TitleSky.h"
#include "Objects/TitleUI.h"
#include "TitleScene.h"

// �R���X�g���N�^
TitleScene::TitleScene()
	: IScene()					// ���N���X�̏�����
	, is_startFlag{ false }		// �J�n�t���O
	, is_menuFlag{ true }		// �I���t���O
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
	Finalize();
}

// ����������
void TitleScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �ϐ��̏�����
	SetSceneValues();
}

// �X�V����
void TitleScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �G�X�P�[�v�ŏI��
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME); }

	// ���S�̍X�V
	m_logo->Update();

	// �I�����ڂ�ύX����
	if (!is_startFlag)
	{
		if (GetFadeValue() >= 0.7f && is_startFlag) return;

		if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Left) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::Right) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::A) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::D))
		{
			is_menuFlag = !is_menuFlag;
			m_stage->AddAccelerate();
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
		}
	}

	// �J�ڐ�����肷��(�Q�[���J�n OR �Q�[���I��)
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (GetFadeValue() >= 0.7f) return;
		is_startFlag = true;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
	// �ړ����I������珈��������
	if (m_logo->IsCanNextFlag())
	{
		ChangeScene(is_menuFlag ? SCENE::SELECT : SCENE::ENDGAME);
	}

	// �X�e�[�W�̍X�V
	m_stage->Update();

	// UI�̍X�V
	m_titleUI->Update(is_menuFlag);

}

// �`�揈��
void TitleScene::Draw()
{
	// �`��֘A
	auto& _states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix _view, _projection;

	// �X�^�[�g���o�̏����͂��̒�����������
	if (is_startFlag)
	{
		m_titleSky->SetStartFlag(true);
		m_logo->SetStartFlag(true);
	}

	// �r���[�s��
	SimpleMath::Vector3 _eye(0.0f, m_logo->GetPosition().y, 8.0f);
	m_titleSky->SetPositionY(_eye.y);
	_view = SimpleMath::Matrix::CreateLookAt(_eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// �v���W�F�N�V�����s��
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// �X�e�[�W��`��
	m_stage->Render(_states, _view, _projection);

	// ���S��`��
	m_logo->Render(_states, _view, _projection);

	// �X�J�C�h�[����`��
	m_titleSky->Render(_states, _view, _projection);

	// UI�̕`��
	m_titleUI->Render(GetFadeValue(), m_logo->IsLogoEndFlag());
}

// �I������
void TitleScene::Finalize()
{
	m_stage->Finalize();
	m_logo->Finalize();
	m_titleSky->Finalize();
	m_titleUI->Finalize();
}

// ��ʁA�f�o�C�X�ˑ��̏�����
void TitleScene::CreateWindowDependentResources()
{
	// �V�X�e���̍쐬
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UI�̏�����
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Create(GetSystemManager(), GetScreenSize());

	// �^�C�g�����S�̐ݒ�
	m_logo = std::make_unique<Logo>(GetFactoryManager(),L"Resources/Models/TitleLogoVer2.cmo");

	// �X�e�[�W�̐ݒ�
	m_stage = std::make_unique<Stage>(GetFactoryManager(), L"Resources/Models/TitleStage.cmo");

	// �X�J�C�h�[���̐ݒ�
	m_titleSky = std::make_unique<TitleSky>(GetFactoryManager(),L"Resources/Models/ShineSky.cmo");
}

// �V�[���ϐ��������֐�
void TitleScene::SetSceneValues()
{
	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);

	// �Q�[�����J�n/�Q�[�����I��
	is_menuFlag = true;
}

