/*
 *	@File	TitleScene.cpp
 *	@Brief	�^�C�g���V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "TitleScene.h"

 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
TitleScene::TitleScene():
	IScene()
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
TitleScene::~TitleScene()
{
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void TitleScene::Initialize()
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
void TitleScene::Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// �L�[���͏����擾����
	GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	m_camera->Update();

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();
	// Space�L�[�ŃV�[���؂�ւ�
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void TitleScene::Draw()
{
	// �f�o�b�O�t�H���g
	GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"TitleScene");
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void TitleScene::Finalize()
{
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
void TitleScene::CreateWindowDependentResources()
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