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
SelectScene::SelectScene()
	: IScene()					// ���N���X�̏�����
	, m_timer{}					// �^�C�}�[
	, m_flashCount{}			// �_�ł̃J�E���^
	, m_stageNum{1}				// �X�e�[�W�ԍ�
	, m_safeStages{}			// ���J���X�e�[�W�ԍ�
	, m_allCoins{}				// ���v�R�C����
	, m_useCoins{}				// �g�p�R�C����
	, m_initCoins{}				// �J�n�R�C����
	, m_targetY{}				// �J�����̃^�[�Q�b�g��Y���W
	, m_mutex{}					// �A�V���N���b�N
	, is_selectEdit{false}		// �X�e�[�W�G�f�B�b�g��I��
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

	// �ϐ��̏�����
	SetSceneValues();

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);
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

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �I��ύX���̉��o
	DirectionSelectChange();

	// �X�e�[�W�̕ύX
	ChangeStageNumber(keyState);

	// UI�̍X�V
	m_userInterface->Update(keyState.Right, keyState.Left);

	// �G�X�P�[�v�ŏI��
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();

	// �R�C���̐���ۑ�
	m_allCoins = m_initCoins - static_cast<int>(m_useCoins);

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (static_cast<int>(GetFadeValue()) != 0) return;

		// �X�e�[�W�ԍ���0�Ȃ�G�f�B�^�ɁA����ȊO�̓v���C��
		if (m_stageNum == 0)
		{
			is_selectEdit = m_allCoins >= STAGE_CREATE_PRICE;

			// ���b�Z�[�W
			if(m_allCoins < STAGE_CREATE_PRICE)
			{
				MessageBox(NULL,
					TEXT("�R�C����10���ȏ�擾���Ă��Ȃ����߁A���̋@�\�͎g�p�ł��܂���B"),
					TEXT("�R�C���s��"), MB_OK);
			}
		}
		else
		{
			ChangeScene(SCENE::PLAY);
		}

		// ���艹��炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	// �R�C�����o
	if (is_selectEdit)
	{
		// �R�C�������Z�b�g����
		m_userInterface->SetAllCoins(m_initCoins - static_cast<int>(m_useCoins));
		m_userInterface->MoveCoins(m_useCoins);

		// �g�p�����x��������J�ڂ���
		if (static_cast<int>(m_useCoins) == STAGE_CREATE_PRICE)
		{
			ChangeScene(SCENE::EDIT);
		}
		else
		{
			m_useCoins += COUNT_SPEED;
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
	m_blocks[m_stageNum] != nullptr ? m_blocks[m_stageNum]->Render(context, states, view, proj, m_timer) : void();

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);

	// UI�̕\��
	m_userInterface->Render(m_stageNum, MAX_STAGE_NUM - 1);

	// �_�ł�����
	if (m_flashCount > MAX_FLASH * 0.5f) return;

	// �e�L�X�g�̈ړ��A�j���[�V����
	SimpleMath::Matrix stageMat = CreateTextMatrix(rotValue);

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
	for (size_t i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
	}
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

	// ��ʃT�C�Y�̊i�[
	float width =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// �X�J�C�h�[�����f�����쐬����
	{
		m_skyDomeModel = ModelFactory::GetCreateModel(device, L"Resources/Models/ShineSky.cmo");
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
	}

	// �u���b�N�̍쐬�𗠂ŏ���
	{
		std::lock_guard<std::mutex>guard(m_mutex);

		m_loadTask = std::async(std::launch::async, [&]() { CreateStages(device); });
	}

	// �X�e�[�W�ԍ��̃��f���̍쐬
	{
		for (int i = 0; i < MAX_STAGE_NUM - m_safeStages; ++i)
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
	}

	// UI�̍쐬
	m_userInterface = std::make_unique<SelectUI>(GetSystemManager(), context, device);
	m_userInterface->Initialize(SimpleMath::Vector2{ width, height });
}

/// <summary>
/// �V�[�����̕ϐ��������֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::SetSceneValues()
{
	// ���グ����
	m_targetY = UP_VALUE;

	// �X�^�[�g��0
	m_flashCount = 0.0f;

	// �R�C�������Z�b�g
	m_userInterface->SetAllCoins(m_allCoins);

	// �g�p�R�C���������Z�b�g
	m_useCoins = 0.0f;

	// �J�n�R�C������ۑ�
	m_initCoins = m_allCoins;

	// �X�e�[�W�G�f�B�b�g�t���O��������
	is_selectEdit = false;
}

/// <summary>
/// �X�e�[�W���f���̃��[�h
/// </summary>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void SelectScene::CreateStages(ID3D11Device1* device)
{
	// �I�𒆂̃X�e�[�W���ɍ쐬
	CreateFirstStage(device);

	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		// �t�@�N�g���[�Ő���
		auto grass   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo"));
		auto coin    = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo"));
		auto cloud   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo"));
		auto gravity = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo"));

		// �쐬����Ă��Ȃ��ꍇ�͍쐬����
		if (!m_blocks[i])
		{
			// �u���b�N�̍쐬
			m_blocks[i] = std::make_unique<Blocks>();
			m_blocks[i]->CreateShader(device);

			// ���f���̎󂯓n��
			m_blocks[i]->CreateModels(std::move(grass),   m_blocks[i]->GRASS);
			m_blocks[i]->CreateModels(std::move(coin),    m_blocks[i]->COIN);
			m_blocks[i]->CreateModels(std::move(cloud),   m_blocks[i]->CLOWD);
			m_blocks[i]->CreateModels(std::move(gravity), m_blocks[i]->GRAVITY);

			// ����������
			m_blocks[i]->Initialize(i);
		}
	}
}

/// <summary>
/// �ŏ��̃X�e�[�W������ɍ쐬����(async�p)
/// </summary>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void SelectScene::CreateFirstStage(ID3D11Device1* device)
{
	// �t�@�N�g���[�Ő���
	auto grass   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo"));
	auto coin    = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo"));
	auto cloud   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo"));
	auto gravity = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo"));

	// �u���b�N�̍쐬
	m_blocks[m_stageNum] = std::make_unique<Blocks>();
	m_blocks[m_stageNum]->CreateShader(device);

	// ���f���̎󂯓n��
	m_blocks[m_stageNum]->CreateModels(std::move(grass),   m_blocks[m_stageNum]->GRASS);
	m_blocks[m_stageNum]->CreateModels(std::move(coin),    m_blocks[m_stageNum]->COIN);
	m_blocks[m_stageNum]->CreateModels(std::move(cloud),   m_blocks[m_stageNum]->CLOWD);
	m_blocks[m_stageNum]->CreateModels(std::move(gravity), m_blocks[m_stageNum]->GRAVITY);

	// ����������
	m_blocks[m_stageNum]->Initialize(m_stageNum);
}

/// <summary>
/// �X�e�[�W�̑I��
/// </summary>
/// <param name="keyState">�L�[�{�[�h</param>
/// <returns>�Ȃ�</returns>
void SelectScene::ChangeStageNumber(Keyboard::State keyState)
{
	// �؂�ւ��\�ȃ^�C�~���O�͂����ŕύX
	if (m_targetY >= UP_VALUE * 0.25f) return;

	if (keyState.Right || keyState.D)
	{
		// �X�e�[�W�ԍ����ő�Ȃ珈�����Ȃ�
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_safeStages) return;

		// �I������炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum++;
		m_flashCount = 0.0f;
	}
	if (keyState.Left || keyState.A)
	{
		// �X�e�[�W�ԍ���0�Ȃ珈�����Ȃ�
		if (m_stageNum == 0) return;

		// �I������炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum--;
		m_flashCount = 0.0f;
	}
}

/// <summary>
/// �Z���N�g�ύX���̉��o
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::DirectionSelectChange()
{
	// �������I����Ă��Ȃ���Ό�������
	if (m_targetY > 1.0f)
	{
		m_targetY -= DOWN_SPEED;
	}
	else if (m_targetY > 0.0f)
	{
		m_targetY -= DOWN_SPEED * 0.5f;
	}
	else
	{
		m_targetY = 0.0f;
	}

	// �t���b�V���J�E���^
	m_flashCount++;
	m_flashCount = m_flashCount > MAX_FLASH * 0.75f ? 0.0f : m_flashCount;
}

/// <summary>
///	�e�L�X�g�̃}�g���b�N�X���쐬
/// </summary>
/// <param name="rotValue">��]����l</param>
/// <returns>�Ȃ�</returns>
SimpleMath::Matrix SelectScene::CreateTextMatrix(const float& rotValue)
{
	SimpleMath::Matrix mat = SimpleMath::Matrix::Identity;
	mat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	mat *= SimpleMath::Matrix::CreateScale(10.0f);
	mat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	return mat;
}

/// <summary>
/// �R�C���Z�b�^�[
/// </summary>
/// <param name="num">���v�R�C����</param>
/// <returns>�Ȃ�</returns>
void SelectScene::SetAllCoins(const int& num)
{
	m_allCoins = num;
}
