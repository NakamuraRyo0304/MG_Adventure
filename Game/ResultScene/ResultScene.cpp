/*
 *	@File	ResultScene.cpp
 *	@Brief  ���U���g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

// �u���b�N
#include "../PlayScene/Objects/Blocks.h"

// UI
#include "Objects/ResultUI.h"

// �����_��(���o�p�Ȃ̂Ń����_���f�o�C�X�͎g��Ȃ�)
#include <random>
#include <time.h>

#include "ResultScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
ResultScene::ResultScene()
	: IScene()					// ���N���X�̏�����
	, m_timer{0.0f}				// ���v
	, m_windowSize{}			// �E�B���h�E�T�C�Y
	, m_coinNum{}				// �R�C���̐�
	, m_clearTime{0.0f}			// �N���A�^�C�����i�[
	, m_saveTime{0.0f}			// �N���A�^�C����ۑ�����ϐ�
	, m_directionTime{0.0f}		// ���o���鎞��
	, m_stageNum{1}				// �w�i�̃X�e�[�W�ԍ�(��������1)
	, m_selectingScene{ 0 }		// ���ݑI�𒆂̃V�[��
{
	// �����_���̐���
	srand(unsigned int(time(0)));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ResultScene::~ResultScene()
{
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);

	// �ϐ��̏�����
	SetSceneValues();

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_RESULT, true);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">����/fps</param>
/// <param name="keyState">�L�[�{�[�h�|�C���^</param>
/// <param name="mouseState">�}�E�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void ResultScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// ���o���Ԃ��J�E���g
	m_directionTime--;

	// ���o������
	if (m_directionTime < 0.0f)
	{
		m_directionTime = 0.0f;
		m_clearTime = 60 - m_saveTime;
	}
	else
	{
		// �����_���Ȓl������
		m_clearTime = static_cast<float>(rand() % 60 + 1);
	}

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// ���j���[�Z���N�g
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
	{
		m_selectingScene++;
		m_selectingScene = m_selectingScene ==  3 ? RETRY : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left))
	{
		m_selectingScene--;
		m_selectingScene = m_selectingScene == -1 ? TITLE : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}

	// UI�̍X�V
	m_userInterface->Update(elapsedTime, static_cast<int>(m_clearTime));
	// ���ݑI�𒆂̃V�[�����Z�b�g
	m_userInterface->SetSelecting(m_selectingScene);
	// �l���R�C�������Z�b�g
	m_userInterface->SetCoins(m_coinNum);

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
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
	// �G�X�P�[�v�ŏI��
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix  view, proj;

	// �r���[�s��
	SimpleMath::Vector3    eye(cosf(m_timer), 20.0f + sinf(m_timer) * 2.0f, 10.0f);
	SimpleMath::Vector3     up(0.0f, 5.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, -10.0f, -5.0f);

	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	m_blocks->Render(context, states, view, proj, m_timer);

	// UI�̕\��
	m_userInterface->Render();
}


/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::Finalize()
{
	// �}�b�v�̌㏈��
	m_blocks->Finalize();

	// UI�̌㏈��
	m_userInterface->Finalize();
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// ��ʃT�C�Y�̊i�[
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �E�B���h�E�T�C�Y���擾
	m_windowSize = SimpleMath::Vector2{ width,height };

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, 45.0f);

	// �u���b�N�̍쐬
	m_blocks = std::make_unique<Blocks>();

	// ���u���b�N�̍쐬
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo")),
		m_blocks->GRASS
	);
	// �R�C���̍쐬
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo")),
		m_blocks->COIN
	);
	// �_�u���b�N�̍쐬
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/MoveBlock.cmo")),
		m_blocks->CLOWD
	);
	// �_���Z�b�g�u���b�N�̍쐬
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo")),
		m_blocks->RECLOWD
	);

	// UI�̍쐬
	m_userInterface = std::make_unique<ResultUI>(GetSystemManager(), context, device);
	m_userInterface->Initialize(SimpleMath::Vector2{ width, height });

}

/// <summary>
/// �V�[�����̕ϐ��������֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::SetSceneValues()
{
	// �^�C�}�[�̕ۑ�
	m_saveTime = m_clearTime;

	// ���o���� �ŏ��̃t�F�[�h���l�����đ��߂Ɏ��
	m_directionTime = 120.0f;
}

/// <summary>
/// �R�C���̃Z�b�^�[
/// </summary>
/// <param name="coinNum">�R�C����</param>
/// <returns>�Ȃ�</returns>
void ResultScene::SetCoinNum(const int& coinNum)
{
	m_coinNum = coinNum;
}
