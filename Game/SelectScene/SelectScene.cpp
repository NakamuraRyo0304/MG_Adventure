/*
 *	@File	SelectScene.cpp
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �}�b�v���[�_�[
#include "Libraries/SystemDatas/MapLoad.h"

// �u���b�N�̍쐬
#include "../PlayScene/Objects/Blocks.h"

#include "SelectScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SelectScene::SelectScene():
	IScene(),
	m_timer{},
	m_stageNum{1},
	m_changeMapMove{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SelectScene::~SelectScene()
{
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetEagleMode(false);	// �J�������_�ړ�

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);
	
	// ���グ����
	m_changeMapMove = UP_VALUE;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">����/fps</param>
/// <param name="keyState">�L�[�{�[�h�|�C���^</param>
/// <param name="mouseState">�}�E�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void SelectScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �������I����Ă��Ȃ���Ό�������
	if (m_changeMapMove > 0.0f)
	{
		m_changeMapMove--;
	}
	else
	{
		m_changeMapMove = 0.0f;
	}


	// �X�e�[�W�ԍ��ύX
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Right))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum++;

		// TODO: [�X�e�[�W�ԍ�]�}�b�v����MAX_STAGE_NUM��ύX�I
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM);

		m_blocks->Initialize(m_stageNum);
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Left))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum--;

		// TODO: [�X�e�[�W�ԍ�]�}�b�v����MAX_STAGE_NUM��ύX�I
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM);

		m_blocks->Initialize(m_stageNum);
	}
	

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		if (m_stageNum == 0)
		{
			// 0�Ԗڂ�I��������X�e�[�W�G�f�B�^�̃V�[���֔��
			GoNextScene(SCENE::EDIT);
		}
		else
		{
			GoNextScene(SCENE::PLAY);
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix world, view, proj;

	// ���[���h�s��
	world = SimpleMath::Matrix::Identity;

	// �r���[�s��
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sin(m_timer), 30.0f, CAMERA_RADIUS * cos(m_timer) * 2.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, m_changeMapMove, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	m_blocks->Render(context, states, view, proj, m_timer);

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);


	//-------------------------------------------------------------------------------------// 

	// �e�L�X�g�̈ړ��A�j���[�V����
	SimpleMath::Matrix stageMat = SimpleMath::Matrix::Identity;
	stageMat *= SimpleMath::Matrix::CreateRotationY(m_timer);
	stageMat *= SimpleMath::Matrix::CreateScale(10.0f);
	stageMat *= SimpleMath::Matrix::CreateTranslation(0.0f, 10.0f, cosf(m_timer) * 2.0f);

	// �X�e�[�W�ԍ��\��
	switch (m_stageNum)
	{
	case 0:
		m_editModel->Draw(context, states, stageMat, view, proj);
		return;
	case 1:
		m_stage1Model->Draw(context, states, stageMat, view, proj);
		return;
	case 2:
		m_stage2Model->Draw(context, states, stageMat, view, proj);
		return;
	case 3:
		m_stage3Model->Draw(context, states, stageMat, view, proj);
		return;
	case 4:
		m_stage4Model->Draw(context, states, stageMat, view, proj);
		return;
	case 5:
		m_stage5Model->Draw(context, states, stageMat, view, proj);
		return;
	default:
		break;
	}

}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::Finalize()
{
	// �}�b�v�̌㏈��
	m_blocks->Finalize();

	// ���f���폜
	ModelFactory::DeleteModel(m_stage1Model);
	ModelFactory::DeleteModel(m_stage2Model);
	ModelFactory::DeleteModel(m_stage3Model);
	ModelFactory::DeleteModel(m_stage4Model);
	ModelFactory::DeleteModel(m_stage5Model);
	ModelFactory::DeleteModel(m_editModel);
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// ��ʃT�C�Y�̊i�[
	float width =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	//-------------------------------------------------------------------------------------// 
	
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

	//-------------------------------------------------------------------------------------// 

	// �X�J�C�h�[�����f�����쐬����	
	m_skyDomeModel = ModelFactory::GetCreateModel(
		device,
		L"Resources/Models/ShineSky.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			// ���Ȕ�������
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	//-------------------------------------------------------------------------------------// 

	// �X�e�[�W�ԍ��̃��f��
	m_editModel   = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
	m_stage1Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage1.cmo");
	m_stage2Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage2.cmo");
	m_stage3Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage3.cmo");
	m_stage4Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage4.cmo");
	m_stage5Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage5.cmo");
}