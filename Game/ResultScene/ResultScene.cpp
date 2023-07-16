/*
 *	@File	ResultScene.cpp
 *	@Brief  ���U���g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

 // CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

// �u���b�N
#include "../PlayScene/Objects/Blocks.h"

#include "ResultScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
ResultScene::ResultScene():
	IScene(),
	m_timer{0.0f},
	m_clearTime{0.0f},
	m_selectNum{RETRY},
	m_stageNum{1}
{
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

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// ���j���[�Z���N�g
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
	{
		m_selectNum++;
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left))
	{
		m_selectNum--;
	}

	// �N�����v
	m_selectNum = UserUtility::Clamp(m_selectNum, static_cast<int>(RETRY), static_cast<int>(TITLE));

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		switch (m_selectNum)
		{
		case RETRY:
			GoNextScene(SCENE::PLAY);
			break;
		case SELECT:
			GoNextScene(SCENE::SELECT);
			break;
		case TITLE:
			GoNextScene(SCENE::TITLE);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ResultScene::Draw()
{
	// �f�o�b�O�t�H���g
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,0 }, 
		L"ResultScene"
	);

	wchar_t sel[10];

	if (m_selectNum == RETRY)
	{
		swprintf_s(sel, 10, L"Retry");
	}
	else if (m_selectNum == SELECT)
	{
		swprintf_s(sel, 10, L"Select");
	}
	else if (m_selectNum == TITLE)
	{
		swprintf_s(sel, 10, L"Title");
	}

	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,20 }, 
		sel
	);

	wchar_t clr[30];
	swprintf_s(clr, 30, L"%f", m_clearTime);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(),
		{ 0,40 },
		clr
	);

	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix world, view, proj;

	// �r���[�s��
	SimpleMath::Vector3    eye(0.0f, 5.0f, 18.0f);
	SimpleMath::Vector3     up(0.0f, 5.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	m_blocks->Render(context, states, view, proj, m_timer);
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
	float width  = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

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


}