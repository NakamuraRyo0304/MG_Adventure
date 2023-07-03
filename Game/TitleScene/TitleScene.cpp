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
	IScene(),
	m_timer{0.0f},
	m_titleLogoModel{},
	m_miniatureModel{}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
TitleScene::~TitleScene()
{
	Finalize();
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void TitleScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	// �J�������W�ݒ�
	GetSystemManager()->GetCamera()->SetEyePosition(SimpleMath::Vector3(0.0f, -20.0f, -20.0f));
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
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

	// Space�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void TitleScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix logoMat, stageMat, skyMat, view, proj;

	// �ړ��A��]�s��
	SimpleMath::Matrix logoTrans, logoRot;
	SimpleMath::Matrix stageTrans, stageRotX,stageRotY;
	SimpleMath::Matrix skyRotY;

	// ���[���h�s��
	logoMat = SimpleMath::Matrix::Identity;
	stageMat = SimpleMath::Matrix::Identity;
	skyMat = SimpleMath::Matrix::Identity;

	// ��]�s��
	logoRot = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);
	stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	stageRotY = SimpleMath::Matrix::CreateRotationY(m_timer) * 0.5f;
	skyRotY = SimpleMath::Matrix::CreateRotationX(m_timer) * 0.2f;

	// �ړ��s��
	logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, 2.0f, cosf(m_timer) * 0.5f);
	stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);

	// ���S
	logoMat *= logoRot * logoTrans;
	// �X�e�[�W
	stageMat *= stageRotY * stageRotX * stageTrans;
	// �X�J�C�h�[��
	skyMat *= skyRotY;

	// �r���[�s��
	SimpleMath::Vector3    eye(0.0f, 0.1f, 8.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// ���f���`��
	m_miniatureModel->Draw(context, states, stageMat, view, proj);	// �X�e�[�W
	m_titleLogoModel->Draw(context, states, logoMat, view, proj);	// ���S
	m_skydomeModel->Draw(context, states, skyMat, view, proj);  	// �X�J�C�h�[��
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void TitleScene::Finalize()
{
	// ���f���̉��
	ModelFactory::DeleteModel(m_titleLogoModel);
	ModelFactory::DeleteModel(m_miniatureModel);
	ModelFactory::DeleteModel(m_skydomeModel);
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
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

	// ���f���̍쐬
	m_titleLogoModel = ModelFactory::GetModel(device, L"Resources/Models/TitleLogo.cmo");
	m_miniatureModel = ModelFactory::GetModel(device, L"Resources/Models/TitleStage.cmo");
	m_skydomeModel   = ModelFactory::GetModel(device, L"Resources/Models/ShineSky.cmo");
}