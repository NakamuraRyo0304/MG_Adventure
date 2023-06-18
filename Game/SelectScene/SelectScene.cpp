/*
 *	@File	SelectScene.cpp
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SelectScene.h"

// �p�x
const float SelectScene::CAMERA_ANGLE = 45.0f;

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
SelectScene::SelectScene():
	IScene(),
	m_tea{},
	m_angle{}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
SelectScene::~SelectScene()
{
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void SelectScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetEagleMode(false);	// �J�������_�ړ�

}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
void SelectScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	m_angle = elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// �X�e�[�W�ԍ��ύX
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Right))
	{
		m_stageNum++;
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Left))
	{
		m_stageNum--;
	}
	
	auto clamp = [](int value, int min, int max)
	{
		if (min > value) return min;
		if (max < value) return max;
		return value;
	};
	m_stageNum = clamp(m_stageNum, 0, 3);

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
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

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void SelectScene::Draw()
{
	// �f�o�b�O�t�H���g
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(),
		{ 0,0 },
		L"SelectScene"
	);

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(),
		{ 0,20 },
		num
	);

	// �J�����p�s��
	DirectX::SimpleMath::Matrix world, view, projection;

	// ���[���h�s��
	world = DirectX::SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	projection = GetSystemManager()->GetCamera()->GetProjection();


	// ���̕`��
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0,0,0);
	switch (m_stageNum)
	{
	case 0:
		m_tea->Draw(world, view, projection, DirectX::Colors::Black);
		break;
	case 1:
		m_tea->Draw(world, view, projection, DirectX::Colors::Red);
		break;
	case 2:
		m_tea->Draw(world, view, projection, DirectX::Colors::Blue);
		break;
	case 3:
		m_tea->Draw(world, view, projection, DirectX::Colors::Green);
		break;
	default:
		break;
	}
	
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void SelectScene::Finalize()
{
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
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

	m_tea = DirectX::GeometricPrimitive::CreateTeapot(context,2.0f);

}