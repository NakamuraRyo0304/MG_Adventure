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

// �����_��(���o�p�Ȃ̂Ń����_���f�o�C�X�͎g��Ȃ�)
#include <random>
#include <time.h>

#include "ResultScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
ResultScene::ResultScene():
	IScene(),
	m_timer{0.0f},				// ���v
	m_clearTime{0.0f},			// �N���A�^�C�����i�[
	m_saveTime{0.0f},			// �N���A�^�C����ۑ�����ϐ�
	m_directionTime{0.0f},		// ���o���鎞��
	m_selectNum{RETRY},			// ���̃V�[���I��
	m_coinNum{0},				// �R�C���̖���
	m_stageNum{1},				// �w�i�̃X�e�[�W�ԍ�(��������1)
	m_retryPos{},				// ���g���C�e�L�X�g�̈ʒu
	m_retryAlpha{},				// ���g���C�e�L�X�g�̓����x
	m_retryScale{},				// ���g���C�e�L�X�g�̑傫��
	m_selectPos{},				// �Z���N�g�e�L�X�g�̈ʒu
	m_selectAlpha{},			// �Z���N�g�e�L�X�g�̓����x
	m_selectScale{},			// �Z���N�g�e�L�X�g�̑傫��
	m_titlePos{},				// �^�C�g���e�L�X�g�̈ʒu
	m_titleAlpha{},				// �^�C�g���e�L�X�g�̓����x
	m_titleScale{},				// �^�C�g���e�L�X�g�̑傫��
	m_coinsPos{},				// �R�C���e�L�X�g�̈ʒu
	m_oneCoiPos{},				// �R�C����1�̈ʂ̍��W
	m_tenCoiPos{},				// �R�C����10�̈ʂ̍��W
	m_windowSize{},				// �E�B���h�E�T�C�Y
	m_clockPos{}				// �^�C�}�[�̍��W
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

	// �^�C�}�[�̕ۑ�
	m_saveTime = m_clearTime;

	// ���o���� �ŏ��̃t�F�[�h���l�����đ��߂Ɏ��
	m_directionTime = 120.0f;
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

	// �A���t�@�l�ƃX�P�[���̕ύX
	switch (m_selectNum)
	{
	case RETRY:
		m_retryAlpha  = 1.0f;
		m_selectAlpha = 0.5f;
		m_titleAlpha  = 0.5f;
		m_retryScale  = 1.1f;
		m_selectScale = 1.0f;
		m_titleScale  = 1.0f;
		break;
	case SELECT:
		m_retryAlpha  = 0.5f;
		m_selectAlpha = 1.0f;
		m_titleAlpha  = 0.5f;
		m_retryScale  = 1.0f;
		m_selectScale = 1.1f;
		m_titleScale  = 1.0f;
		break;
	case TITLE:
		m_retryAlpha  = 0.5f;
		m_selectAlpha = 0.5f;
		m_titleAlpha  = 1.0f;
		m_retryScale  = 1.0f;
		m_selectScale = 1.0f;
		m_titleScale  = 1.1f;
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

	//-------------------------------------------------------------------------------------// 

	// �摜�̊g�嗦���E�B���h�E�����ƂɌv�Z
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// ��ʂ𔖈Â�����
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"BLIND",										// �o�^�L�[
			SimpleMath::Vector2::Zero,						// ���W
			{ 1.0f,1.0f,1.0f,0.5f },						// �F
			1.0f,											// �g�嗦
			SimpleMath::Vector2::Zero						// ���S�ʒu
		);

	// �R�C������
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"COINS",										// �o�^�L�[
			m_coinsPos,										// ���W
			{ 1.0f,1.0f,1.0f,1.0f },						// �F
			IMAGE_RATE * imageScale * 1.0f,					// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }					// ���S�ʒu
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


	// ���v�摜
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Clock",										// �o�^�L�[
			m_clockPos,										// ���W
			{ 1.0f,1.0f,1.0f,1.0f },						// �F
			DEFAULT_RATE * imageScale,						// �g�嗦
			{ IMAGE_CENTER,IMAGE_CENTER }					// ���S�ʒu
		);


	//-------------------------------------------------------------------------------------// 
	// �b�����v�Z
	int sec = static_cast<int>(m_clearTime);

	// �ꌅ�ڂ̐�����\��
	RenderDigit(sec % 10, m_oneSecPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	// �\�̌��̐�����\��
	RenderDigit((sec / 10) % 10, m_tenSecPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	//-------------------------------------------------------------------------------------// 

	// �ꌅ�ڂ̐�����\��
	RenderDigit(m_coinNum % 10, m_oneCoiPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	// �\�̌��̐�����\��
	RenderDigit((m_coinNum / 10) % 10, m_tenCoiPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);
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
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"COINS", L"Resources/Textures/FONT/COINS.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/ResultBack.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Clock", L"Resources/Textures/Clock.dds", device);

	// �䗦���v�Z
	float span = width / FULL_SCREEN_SIZE.x;

	// �X�v���C�g�̈ʒu���v�Z
	m_oneSecPos = { (NUM_OFFSET + static_cast<float>(SPRITE_SIZE / 2)) * span, 100.0f * span };
	m_tenSecPos = { (NUM_OFFSET - static_cast<float>(SPRITE_SIZE / 2)) * span, 100.0f * span };
	m_oneCoiPos = { (NUM_OFFSET + static_cast<float>(SPRITE_SIZE / 2)) * span, 440.0f * span };
	m_tenCoiPos = { (NUM_OFFSET - static_cast<float>(SPRITE_SIZE / 2)) * span, 440.0f * span };

	// ���W���
	m_coinsPos  = { TEXT_OFFSET * span, 400.0f * span };
	m_retryPos  = { TEXT_OFFSET * span, 700.0f * span };
	m_selectPos = { TEXT_OFFSET * span, 800.0f * span };
	m_titlePos  = { TEXT_OFFSET * span, 900.0f * span };
	m_clockPos  = { (SPRITE_SIZE + width * span) / 2,150.0f * span };

	// ���W�␳
	CorrectionOffset(width, span);
}

/// <summary>
/// ������`�悷��
/// </summary>
/// <param name="digit">�`�悷�鐔��</param>
/// <param name="position">���W</param>
/// <param name="scale">�g�嗦</param>
/// <param name="digitWidth">�����̕�</param>
/// <param name="digitHeight">�����̍���</param>
/// <returns>�Ȃ�</returns>
void ResultScene::RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, float scale, int digitWidth, int digitHeight)
{
	// �X�v���C�g�̈ʒu���v�Z
	float spritePosX = position.x * scale;
	float spritePosY = position.y * scale;

	// �X�v���C�g�̒��S�ʒu���v�Z
	SimpleMath::Vector2 center = { spritePosX  * scale / 2.0f, spritePosY  * scale / 2.0f };

	// �؂���ʒu�̐ݒ�
	RECT_U rect;

	// �؂���J�n�ʒu��ݒ�(��)
	rect.left = digit * digitWidth;

	// �؂���I���ʒu��ݒ�(��)
	rect.right = rect.left + digitWidth;

	// �摜�c����ݒ�
	rect.bottom = digitHeight;

	// �����\��
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"Number",                         // �o�^�L�[
		position + center,                 // ���W
		{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
		scale,                             // �g�嗦
		center,                            // ���S�ʒu
		rect                               // �؂���ʒu
	);
}

/// <summary>
/// �I�t�Z�b�g�l��ύX����֐�
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="span">�ő�T�C�Y�Ƃ̔䗦</param>
/// <returns>�Ȃ�</returns>
void ResultScene::CorrectionOffset(float width ,float span)
{
	// ���W�␳
	if (static_cast<int>(width) != 1280) return;

	m_oneSecPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_tenSecPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_oneCoiPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_tenCoiPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_clockPos.x  += static_cast<float>(SPRITE_SIZE * 2.95f * span);
}
