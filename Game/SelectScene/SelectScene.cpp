/*
 *	@File	SelectScene.cpp
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include <thread>
#include <mutex>

// �}�b�v���[�_�[
#include "Libraries/SystemDatas/MapLoad.h"

// �u���b�N�̍쐬
#include "../PlayScene/Objects/Blocks.h"

// UI
#include "Objects/SelectUI.h"

#include "SelectScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SelectScene::SelectScene():
	IScene(),
	m_timer{},				// �^�C�}�[
	m_flashCount{},			// �_�ł̃J�E���^
	m_stageNum{1},			// �X�e�[�W�ԍ�
	m_targetY{},			// �J�����̃^�[�Q�b�g��Y���W
	m_mutex{}				// ���b�N
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SelectScene::~SelectScene()
{
	Finalize();
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

	// �J�������_�ړ���؂�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// ���グ����
	m_targetY = UP_VALUE;

	// �X�^�[�g��0
	m_flashCount = 0.0f;
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
	if (m_targetY > 1.0f)
	{
		m_targetY -= DOWN_SPEED;
	}
	else if(m_targetY > 0.0f)
	{
		m_targetY -= DOWN_SPEED * 0.5f;
	}
	else
	{
		m_targetY = 0.0f;
	}

	// �؂�ւ��\�ȃ^�C�~���O�͂����ŕύX
	if (m_targetY >= UP_VALUE * 0.25f) return;

	// �t���b�V���J�E���^
	m_flashCount++;
	m_flashCount = m_flashCount > MAX_FLASH * 0.75f ? 0.0f : m_flashCount;

	// �X�e�[�W�ԍ��ύX
	if(keyState.Right)
	{
		// �X�e�[�W�ԍ����ő�Ȃ珈�����Ȃ�
		if (m_stageNum == MAX_STAGE_NUM - 1) return;

		m_targetY = UP_VALUE;
		m_stageNum++;
		m_flashCount = 0.0f;
	}
	if (keyState.Left)
	{
		// �X�e�[�W�ԍ���0�Ȃ珈�����Ȃ�
		if (m_stageNum == 0) return;

		m_targetY = UP_VALUE;
		m_stageNum--;
		m_flashCount = 0.0f;
	}

	// UI�̍X�V
	m_userInterface->Update(
		elapsedTime,	// ����
		keyState.Right,	// �E�L�[����
		keyState.Left	// ���L�[����
	);

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �X�e�[�W�ԍ���0�Ȃ�G�f�B�^�ɁA����ȊO�̓v���C��
		m_stageNum == 0 ? ChangeScene(SCENE::EDIT) : ChangeScene(SCENE::PLAY);
	}

	// �G�X�P�[�v�ŏI��
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape))
	{
		ExitApp();
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
	SimpleMath::Matrix view, proj;

	// ��]�ʂ��v�Z
	float rotValue = m_timer * 0.5f;
	// �㉺�ړ��ʂ��v�Z
	float verticalValue = sinf(m_timer * 1.5f) * 1.5f;

	// �r���[�s��
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sinf(rotValue),		// X:��](�X�e�[�W�Ƌt��])
						       CAMERA_POS_Y  + verticalValue,		// Y:�ړ�(�㉺)
						       CAMERA_RADIUS * cosf(rotValue));		// Z:��](�X�e�[�W�Ƌt��])
	SimpleMath::Vector3 target(0.0f, m_targetY, 0.0f);

	view = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	if(m_blocks[m_stageNum] != nullptr)
	{
		m_blocks[m_stageNum]->Render(context, states, view, proj, m_timer);
	}

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);


	//-------------------------------------------------------------------------------------//

	// UI�̕\��
	m_userInterface->Render(m_stageNum);

	// �_�ł�����
	if (m_flashCount > MAX_FLASH * 0.5f) return;

	// �e�L�X�g�̈ړ��A�j���[�V����
	SimpleMath::Matrix stageMat = SimpleMath::Matrix::Identity;
	stageMat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	stageMat *= SimpleMath::Matrix::CreateScale(10.0f);
	stageMat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	// �X�e�[�W�ԍ��\��
	m_stageModels[m_stageNum]->Draw(context, states, stageMat, view, proj);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::Finalize()
{
	// ���f���폜
	m_blocks[0]->Finalize();
	m_userInterface->Finalize();
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

	// �u���b�N�̍쐬�𗠂ŏ���
	{
		std::lock_guard<std::mutex>guard(m_mutex);

		m_loadTask = std::async(std::launch::async, [&]() { LoadStage(device); });
	}

	// �X�e�[�W�ԍ��̃��f��
	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		// 0�Ԗڂ̓G�f�B�^�̕���
		if (i == 0)
		{
			m_stageModels[0] = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
		}
		else
		{
			std::wstring path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
			m_stageModels[i] = ModelFactory::GetCreateModel(device, path.c_str());
		}
	}

	// UI�̍쐬
	m_userInterface = std::make_unique<SelectUI>(GetSystemManager(), context, device);
	m_userInterface->Initialize(SimpleMath::Vector2{ width, height });
}

/// <summary>
/// �X�e�[�W���f���̃��[�h
/// </summary>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void SelectScene::LoadStage(ID3D11Device1* device)
{
	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i] = std::make_unique<Blocks>();

		// ���u���b�N�̍쐬
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo")),
			m_blocks[i]->GRASS
		);
		// �R�C���̍쐬
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo")),
			m_blocks[i]->COIN
		);
		// �_�u���b�N�̍쐬
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/MoveBlock.cmo")),
			m_blocks[i]->CLOWD
		);
		// �_���Z�b�g�u���b�N�̍쐬
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo")),
			m_blocks[i]->RECLOWD
		);

		// ����������
		m_blocks[i]->Initialize(i);
	}
}
