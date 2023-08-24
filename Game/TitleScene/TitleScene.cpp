/*
 *	@File	TitleScene.cpp
 *	@Brief	�^�C�g���V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Objects/TitleUI.h"

#include "TitleScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
TitleScene::TitleScene():
	IScene(),
	m_timer{0.0f},
	m_titleLogoModel{},
	m_miniatureModel{},
	m_moveY{0.0f},
	is_startFlag{false},
	m_logoMoveScale{},
	is_gameFlag{true}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
TitleScene::~TitleScene()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	// �J�������W�ݒ�
	GetSystemManager()->GetCamera()->SetEyePosition(SimpleMath::Vector3(0.0f, -20.0f, -20.0f));

	// ���S�̑傫��
	m_logoMoveScale = 1.0f;

	// �Q�[�����J�n/�Q�[�����I��
	is_gameFlag = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">����/fps</param>
/// <param name="keyState">�L�[�{�[�h�|�C���^</param>
/// <param name="mouseState">�}�E�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void TitleScene::Update(const float& elapsedTime,Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// ���肵�Ă��Ȃ���ΑI����؂�ւ���
	if (!is_startFlag)
	{
		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left))
		{
			is_gameFlag = true;
		}
		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
		{
			is_gameFlag = false;
		}
	}

	// �J�ڐ�����肷��(�Q�[���J�n OR �Q�[���I��)
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		is_startFlag = true;
	}
	if (is_startFlag)
	{
		m_moveY++;

		// ���o���I�������J��
		if (m_moveY > MAX_HEIGHT)
		{
			if (is_gameFlag)	// Start��I��������Z���N�g�V�[���ɍs��
			{
				ChangeScene(SCENE::SELECT);
			}
			else				// Exit��I��������Q�[�����I������
			{
				ChangeScene(SCENE::ENDGAME);
			}
		}
	}

	// UI�̍X�V
	m_titleUI->Update(is_gameFlag);

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
void TitleScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix logoMat, stageMat, skyMat, view, proj;

	// �ړ��A��]�s��
	SimpleMath::Matrix logoTrans, logoRot;
	SimpleMath::Matrix stageTrans, stageRotX, stageRotY, stageScale;
	SimpleMath::Matrix skyTrans,skyRotX;

	// ���[���h�s��
	logoMat = SimpleMath::Matrix::Identity;
	stageMat = SimpleMath::Matrix::Identity;
	skyMat = SimpleMath::Matrix::Identity;

	// ��]�s��
	logoRot = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);
	stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	stageRotY = SimpleMath::Matrix::CreateRotationY(m_timer * 0.5f);
	skyRotX = SimpleMath::Matrix::CreateRotationX(m_timer * 2.0f);

	// �ړ��s��
	logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, 2.0f, cosf(m_timer) * 0.5f);
	stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	skyTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_moveY, 0.0f);

	// �X�P�[���s��
	stageScale = SimpleMath::Matrix::CreateScale(1.2f);

	// ���S
	logoMat *= logoRot * logoTrans;
	// �X�e�[�W
	stageMat *= stageScale * stageRotY * stageRotX * stageTrans;
	// �X�J�C�h�[��
	skyMat *= skyRotX;

	// �X�^�[�g���o�̏����͂��̒�
	if (is_startFlag)
	{
		// �X�J�C�h�[���̉�]�ƈړ�
		skyMat *= skyRotX;
		skyMat *= skyTrans;

		// ���S���ꏏ�ɓ���
		m_logoMoveScale *= 1.05f;
		logoMat *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// �r���[�s��
	SimpleMath::Vector3    eye(0.0f, 0.1f + m_moveY, 8.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// ���C�g�̐ݒ�
	SimpleMath::Vector3 lightDirection(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// �A�}�`���A���f���ݒ�
	m_miniatureModel->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�I��
				lights->SetLightEnabled(0, true);
				lights->SetLightEnabled(1, true);
				lights->SetLightEnabled(2, true);

				// ���C�g�̕�����ݒ�
				lights->SetLightDirection(0, lightDirection);
				lights->SetLightDirection(1, lightDirection);
				lights->SetLightDirection(2, lightDirection);

				// ���C�g�̐F��ݒ�
				lights->SetLightDiffuseColor(0, lightColor);
				lights->SetLightDiffuseColor(1, lightColor);
				lights->SetLightDiffuseColor(2, lightColor);
			}
		});
	m_miniatureModel->Draw(context, states, stageMat, view, proj);	// �X�e�[�W
	m_titleLogoModel->Draw(context, states, logoMat, view, proj);	// ���S
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);  	// �X�J�C�h�[��

	// UI�̕`��
	m_titleUI->Render();
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Finalize()
{
	// ���f���̉��
	ModelFactory::DeleteModel(m_titleLogoModel);
	ModelFactory::DeleteModel(m_miniatureModel);
	ModelFactory::DeleteModel(m_skyDomeModel);

	// UI�̏I������
	m_titleUI->Finalize();
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::CreateWindowDependentResources()
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

	// ���f���̍쐬---------------------------------------------------------------------------------

	// �^�C�g�����S
	m_titleLogoModel = ModelFactory::GetCreateModel(device, L"Resources/Models/TitleLogo.cmo");

	// �X�e�[�W���f��
	m_miniatureModel = ModelFactory::GetCreateModel(device, L"Resources/Models/TitleStage.cmo");

	// �X�J�C�h�[��
	m_skyDomeModel = ModelFactory::GetCreateModel(device, L"Resources/Models/ShineSky.cmo");
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			// ���C�e�B���O
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, true);
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


	// UI�̏�����
	m_titleUI = std::make_unique<TitleUI>(SimpleMath::Vector2{ width, height });
	m_titleUI->Create(GetSystemManager(), context, device);
}