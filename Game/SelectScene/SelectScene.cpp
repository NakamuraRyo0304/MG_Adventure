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
	m_flashCount{},
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

	// �J�������_�ړ���؂�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// ���グ����
	m_changeMapMove = UP_VALUE;

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
	if (m_changeMapMove > 1.0f)
	{
		m_changeMapMove -= DOWN_SPEED;
	}
	else if(m_changeMapMove > 0.0f)
	{
		m_changeMapMove -= DOWN_SPEED / 2;
	}
	else
	{
		m_changeMapMove = 0.0f;
	}


	// �X�e�[�W�ԍ��ύX
	if (static_cast<int>(m_changeMapMove) >= UP_VALUE / 2) return;

	// �t���b�V���J�E���^
	m_flashCount++;
	m_flashCount = m_flashCount > MAX_FLASH * 2 / 3 ? 0.0f : m_flashCount;

	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Right))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum++;
		m_flashCount = 0.0f;

		// TODO: [�X�e�[�W�ԍ�]�}�b�v����MAX_STAGE_NUM��ύX�I
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM - 1);
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Left))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum--;
		m_flashCount = 0.0f;

		// TODO: [�X�e�[�W�ԍ�]�}�b�v����MAX_STAGE_NUM��ύX�I
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM - 1);
	}

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		if (m_stageNum == 0)
		{
			// 0�Ԗڂ�I��������X�e�[�W�G�f�B�^�̃V�[���֔��
			ChangeScene(SCENE::EDIT);
		}
		else
		{
			ChangeScene(SCENE::PLAY);
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
	SimpleMath::Matrix view, proj;

	// ��]�ʂ��v�Z
	float rotValue = m_timer * 0.5f;
	// �㉺�ړ��ʂ��v�Z
	float verticalValue = sinf(m_timer * 2.0f) * 2.0f;

	// �r���[�s��
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sin(rotValue), 30.0f + verticalValue, CAMERA_RADIUS * cos(rotValue));
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, m_changeMapMove, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

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

	// �_�ł�����
	if (static_cast<int>(m_flashCount) > static_cast<int>(MAX_FLASH) / 2) return;

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
	// �}�b�v�̌㏈��
	// ���f���폜
	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
		ModelFactory::DeleteModel(m_stageModels[i]);
	}
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
	m_loadTask = std::async(std::launch::async, [&]() { LoadStage(device); });

	// �X�e�[�W�ԍ��̃��f��
	for (int i = 0; i < MAX_STAGE_NUM; i++)
	{
		// 0�Ԗڂ̓G�f�B�^�̕���
		if (i == 0)
		{
			m_stageModels[0] = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
		}
		else
		{
			std::wstring stageNumber = std::to_wstring(i);
			std::wstring path = L"Resources/Models/Stage" + stageNumber + L".cmo";
			m_stageModels[i] = ModelFactory::GetCreateModel(device, path.c_str());
		}
	}
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
