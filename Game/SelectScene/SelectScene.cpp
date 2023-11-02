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
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::Update()
{
	// �C���v�b�g�̍X�V
	auto& _input = Input::GetInstance();
	auto _key = Keyboard::Get().GetState();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �I��ύX���̉��o
	DirectionSelectChange();

	// �X�e�[�W�̕ύX
	ChangeStageNumber();

	// UI�̍X�V
	m_selectUI->Update(_timer, (_key.D || _key.Right), (_key.A || _key.Left));

	// �G�X�P�[�v�ŏI��
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME); }

	// �R�C���̐���ۑ�
	m_allCoins = m_initCoins - static_cast<int>(m_useCoins);

	// Space�L�[�ŃV�[���؂�ւ�
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (GetFadeValue() >= 0.7f) return;

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
		m_selectUI->SetAllCoins(m_initCoins - static_cast<int>(m_useCoins));
		m_selectUI->MoveCoins(m_useCoins);

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
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����p�s��
	SimpleMath::Matrix _view, _proj;

	// ��]�ʂ��v�Z
	float _rotValue = _timer * 0.5f;
	// �㉺�ړ��ʂ��v�Z
	float _verticalValue = sinf(_timer * 1.5f) * 1.5f;

	// �r���[�s��
	SimpleMath::Vector3 _eye(CAMERA_RADIUS * sinf(_rotValue),		// X:��](�X�e�[�W�Ƌt��])
							 CAMERA_POS_Y + _verticalValue,			// Y:�ړ�(�㉺)
							 CAMERA_RADIUS * cosf(_rotValue));		// Z:��](�X�e�[�W�Ƌt��])
	SimpleMath::Vector3 _target(0.0f, m_targetY, 0.0f);

	_view = SimpleMath::Matrix::CreateLookAt(_eye, _target, SimpleMath::Vector3::Up);

	// �v���W�F�N�V�����s��
	_proj = GetSystemManager()->GetCamera()->GetProjection();

	// �}�b�v�̕`��
	m_blocks[m_stageNum] != nullptr ? // �쐬�ς݂Ȃ�`�悷��
		m_blocks[m_stageNum]->Render(_states, _view, _proj, _timer,
			SimpleMath::Vector3{ 1.0f,-1.0f,-1.0f }) : void();

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(_context, _states, skyMat, _view, _proj);

	// �_�ł�����
	if (m_flashCount < MAX_FLASH * 0.5f)
	{
		// �e�L�X�g�̈ړ��A�j���[�V����
		SimpleMath::Matrix stageMat = CreateTextMatrix(_rotValue);

		// �X�e�[�W�ԍ��\��
		m_stageModels[m_stageNum]->Draw(_context, _states, stageMat, _view, _proj);
	}

	// UI�̕`��
	m_selectUI->Render(GetFadeValue(), m_stageNum, MAX_STAGE_NUM - 1);
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
	m_selectUI->Finalize();
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto _device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UI�̍쐬
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_selectUI = std::make_unique<SelectUI>();
	m_selectUI->Create(GetSystemManager(), _device, GetScreenSize());

	// �X�J�C�h�[�����f�����쐬����
	{
		m_skyDomeModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/ShineSky.cmo");
		m_skyDomeModel->UpdateEffects([](IEffect* effect)
			{
				auto _lights = dynamic_cast<IEffectLights*>(effect);
				if (_lights)
				{
					_lights->SetLightEnabled(0, false);
					_lights->SetLightEnabled(1, false);
					_lights->SetLightEnabled(2, false);
				}
				// ���Ȕ�������
				auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
				if (_basicEffect)
				{
					_basicEffect->SetEmissiveColor(Colors::White);
				}
			}
		);
	}

	// �u���b�N�̍쐬�𗠂ŏ���
	{
		std::lock_guard<std::mutex>_guard(m_mutex);

		m_loadTask = std::async(std::launch::async, [&]() { CreateStages(_device); });
	}

	// �X�e�[�W�ԍ��̃��f���̍쐬
	{
		for (int i = 0; i < MAX_STAGE_NUM - m_safeStages; ++i)
		{
			// 0�Ԗڂ̓G�f�B�^�̕���
			if (i == 0)
			{
				m_stageModels[0] = ModelFactory::GetCreateModel(_device, L"Resources/Models/StageEdit.cmo");
			}
			else
			{
				std::wstring _path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
				m_stageModels[i] = ModelFactory::GetCreateModel(_device, _path.c_str());
			}
		}
	}
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
	m_selectUI->SetAllCoins(m_allCoins);

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
		auto _grass   = ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo");
		auto _coin    = ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo");
		auto _cloud   = ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo");
		auto _gravity = ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo");

		// �쐬����Ă��Ȃ��ꍇ�͍쐬����
		if (!m_blocks[i])
		{
			// �u���b�N�̍쐬
			m_blocks[i] = std::make_unique<Blocks>();
			m_blocks[i]->CreateShader();

			// ���f���̎󂯓n��
			m_blocks[i]->CreateModels(std::move(_grass),   m_blocks[i]->GRASS);
			m_blocks[i]->CreateModels(std::move(_coin),    m_blocks[i]->COIN);
			m_blocks[i]->CreateModels(std::move(_cloud),   m_blocks[i]->CLOWD);
			m_blocks[i]->CreateModels(std::move(_gravity), m_blocks[i]->GRAVITY);

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
	auto _grass   = ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo");
	auto _coin    = ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo");
	auto _cloud   = ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo");
	auto _gravity = ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo");

	// �u���b�N�̍쐬
	m_blocks[m_stageNum] = std::make_unique<Blocks>();
	m_blocks[m_stageNum]->CreateShader();

	// ���f���̎󂯓n��
	m_blocks[m_stageNum]->CreateModels(std::move(_grass),   m_blocks[m_stageNum]->GRASS);
	m_blocks[m_stageNum]->CreateModels(std::move(_coin),    m_blocks[m_stageNum]->COIN);
	m_blocks[m_stageNum]->CreateModels(std::move(_cloud),   m_blocks[m_stageNum]->CLOWD);
	m_blocks[m_stageNum]->CreateModels(std::move(_gravity), m_blocks[m_stageNum]->GRAVITY);

	// ����������
	m_blocks[m_stageNum]->Initialize(m_stageNum);
}

/// <summary>
/// �X�e�[�W�̑I��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SelectScene::ChangeStageNumber()
{
	// �؂�ւ��\�ȃ^�C�~���O�͂����ŕύX
	if (m_targetY >= UP_VALUE * 0.25f) return;

	// �C���v�b�g�̍X�V
	auto _input = Keyboard::Get().GetState();

	if (_input.Right || _input.D)
	{
		// �X�e�[�W�ԍ����ő�Ȃ珈�����Ȃ�
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_safeStages) return;

		// �I������炷
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum++;
		m_flashCount = 0.0f;
	}
	if (_input.Left || _input.A)
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
	SimpleMath::Matrix _mat = SimpleMath::Matrix::Identity;
	_mat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	_mat *= SimpleMath::Matrix::CreateScale(10.0f);
	_mat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	return _mat;
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
