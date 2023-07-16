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
	m_stageNum{1},
	m_retryPos{},
	m_retryAlpha{},
	m_retryScale{},
	m_selectPos{},
	m_selectAlpha{},
	m_selectScale{},
	m_titlePos{},
	m_titleAlpha{},
	m_titleScale{},
	m_windowSize{}
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
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Down))
	{
		m_selectNum++;
		if (m_selectNum == 3) // ���[�v����
		{
			m_selectNum = RETRY;
		}
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Up))
	{
		m_selectNum--;
		if (m_selectNum < 0) // ���[�v����
		{
			m_selectNum = TITLE;
		}
	}

	// �A���t�@�l�̕ύX
	switch (m_selectNum)
	{
	case RETRY:
		m_retryAlpha = 1.0f;
		m_selectAlpha = 0.5f;
		m_titleAlpha = 0.5f;
		m_retryScale = 1.1f;
		m_selectScale = 1.0f;
		m_titleScale = 1.0f;
		break;
	case SELECT:
		m_retryAlpha = 0.5f;
		m_selectAlpha = 1.0f;
		m_titleAlpha = 0.5f;
		m_retryScale = 1.0f;
		m_selectScale = 1.1f;
		m_titleScale = 1.0f;
		break;
	case TITLE:
		m_retryAlpha = 0.5f;
		m_selectAlpha = 0.5f;
		m_titleAlpha = 1.0f;
		m_retryScale = 1.0f;
		m_selectScale = 1.0f;
		m_titleScale = 1.1f;
		break;
	default:
		break;
	}

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

	//-------------------------------------------------------------------------------------// 

	// �摜�̊g�嗦���E�B���h�E�����ƂɌv�Z
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ��ʂ𔖈Â�����
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"BLIND",											// �o�^�L�[
		SimpleMath::Vector2::Zero,							// ���W
		{ 1.0f,1.0f,1.0f,0.5f },							// �F
		1.0f,												// �g�嗦
		SimpleMath::Vector2::Zero							// ���S�ʒu
	);

	// ���g���C����
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"RETRY",										// �o�^�L�[
			m_retryPos,										// ���W
			{ 1.0f,1.0f,1.0f,m_retryAlpha },				// �F
			IMAGE_RATE * imageScale * m_retryScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }					// ���S�ʒu
		);

	// �Z���N�g����
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"SELECT",										// �o�^�L�[
			m_selectPos,									// ���W
			{ 1.0f,1.0f,1.0f,m_selectAlpha },				// �F
			IMAGE_RATE * imageScale * m_selectScale,		// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }					// ���S�ʒu
		);

	// �^�C�g������
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"TITLE",										// �o�^�L�[
			m_titlePos,										// ���W
			{ 1.0f,1.0f,1.0f,m_titleAlpha },				// �F
			IMAGE_RATE * imageScale * m_titleScale,			// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }					// ���S�ʒu
		);
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

	// �摜�̐ݒ�
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(context);
	// �摜��o�^
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"RETRY", L"Resources/Textures/FONT/RETRY.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"TITLE", L"Resources/Textures/FONT/TITLE.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/ResultBack.dds", device);

	// �䗦���v�Z
	float span = static_cast<float>(width) / FULL_SCREEN_SIZE.x;

	// ���W���
	m_retryPos  = { 960.0f * span, 500.0f * span };
	m_selectPos = { 960.0f * span, 600.0f * span };
	m_titlePos  = { 960.0f * span, 700.0f * span };
}