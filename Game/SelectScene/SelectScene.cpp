/*
 *	@File	SelectScene.cpp
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SelectScene.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
SelectScene::SelectScene():
	IScene()
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

	m_camera->SetMoveMode(true);				// �J�������W�ړ�
	m_camera->SetEagleMode(false);				// �J�������_�ړ�
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
void SelectScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// �L�[���͏����擾����
	GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	m_camera->Update();

	// �X�e�[�W�ԍ��ύX
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Right)) m_stageNum++;
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Left))  m_stageNum--;
	auto clamp = [](int value,int min,int max)
	{
		if (min > value) return min;
		if (max < value) return max;
		return value;
	};
	m_stageNum = clamp(m_stageNum, 1, 3);

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();
	// Space�L�[�ŃV�[���؂�ւ�
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::PLAY);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void SelectScene::Draw()
{
	// �f�o�b�O�t�H���g
	GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"SelectScene");

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,20 }, num);
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
	auto device  = GetDeviceResources()->GetD3DDevice();
	auto context = GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	CreateUnique(device, context);
	GetString()->CreateString(device, context);

	// ��ʃT�C�Y�̊i�[
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);

	// �J�����̐ݒ�
	m_camera->GetProjection(width, height, 45.0f);

}