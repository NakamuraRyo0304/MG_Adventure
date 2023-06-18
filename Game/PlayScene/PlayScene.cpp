/*
 *	@File	PlayScene.cpp
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

// �R���C�_�[�N���X
#include "../../Libraries/SystemDatas/Collider.h"

// �v���C���N���X
#include "Objects/Player.h"

#include "PlayScene.h"

 //--------------------------------------------------------//
 //�R���X�g���N�^                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_mapObj{},						// �}�b�v�̃u���b�N
	m_map{},						// �}�b�v
	m_colObjList{},					// �����Ă���I�u�W�F�N�g�̊i�[
	is_boxCol{},					// �����̓����蔻��
	m_playerPos{},					// �v���C���̈ʒu
	m_grassModel{ nullptr },		// ���u���b�N�̃��f��
	m_coinModel{ nullptr },			// �R�C���u���b�N�̃��f��
	m_clowdModel{ nullptr },		// �_�u���b�N�̃��f��
	test_count{}
{

}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
PlayScene::~PlayScene()
{
	Finalize();
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void PlayScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetEagleMode(true);		// �J�������_�ړ�

	// �}�b�v�ǂݍ���
	LoadMap(GetStageNum());

	// �v���C���̏�����
	m_player->Initialize();
	m_player->SetPosition(DirectX::SimpleMath::Vector3{ 0.0f,5.0f,0.0f });

	// ����̏�����
	m_colObjList.clear();
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
// ��P�����F����(60FPS = 1sec) / ��Q�����F�L�[�{�[�h�̃|�C���^ / ��R�����F�}�E�X�̃|�C���^
void PlayScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �v���C���̍X�V
	m_player->Update(keyState);

	// �����蔻��̍X�V
	DoBoxCollision();

	// �Փ˂����I�u�W�F�N�g���Ƃɉ����߂��������s��
	for (auto& obj : m_colObjList)
	{
		ApplyPushBack(obj);
	}

	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::PLAY);
	}

	// ESC�L�[�ŏI��
	if (keyState.Escape) ExitApp();

	// Enter�L�[�ŃV�[���؂�ւ�
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Enter))
	{
		GoNextScene(SCENE::RESULT);
	}
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void PlayScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	DirectX::SimpleMath::Matrix world, view, proj;

	// ���[���h�s��
	world = DirectX::SimpleMath::Matrix::Identity;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �v���C���̕`��
	m_player->Render(context, states, view, proj);

	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		DirectX::SimpleMath::Matrix boxMat =
			DirectX::SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(context, states, boxMat, view, proj);
		}
	}

	DebugLog(view, proj);
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void PlayScene::Finalize()
{
	// �v���C���̌㏈��
	m_player->Finalize();

	// �}�b�v�̉��
	m_mapObj.clear();
	std::vector<Object>(m_mapObj).swap(m_mapObj);

	// ����p�z������
	m_colObjList.clear();
	std::vector<Object>(m_colObjList).swap(m_colObjList);

	// ���f���̃����[�X
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
}

//--------------------------------------------------------//
//��ʈˑ��̏�����                                        //
//--------------------------------------------------------//
void PlayScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique(device, context);

	// ��ʃT�C�Y�̊i�[
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// �����̐ݒ�
	GetSystemManager()->GetString()->CreateString(device, context);

	// ���f�����쐬����
	m_grassModel = ModelFactory::GetModel(						// ���u���b�N
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetModel(						// �R�C���u���b�N
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetModel(						// �_�u���b�N
		device,
		L"Resources/Models/Clowd.cmo"
	);

	// �v���C���̍쐬
	std::unique_ptr<DirectX::Model> playerModel = ModelFactory::GetModel(
		device,
		L"Resources/Models/TestPlayer.cmo"
	);
	// ���f���f�[�^��n��(move�K�v)
	m_player = std::make_unique<Player>(std::move(playerModel));

}

//--------------------------------------------------------//
//�����Ă����烊�X�g�ɒǉ�����                            //
//--------------------------------------------------------//
void PlayScene::DoBoxCollision()
{
	// �Փ˂����I�u�W�F�N�g���X�g��������
	m_colObjList.clear(); 

	// �����蔻������
	for (auto& obj : m_mapObj)
	{
		// ��������
		is_boxCol.PushBox(
			m_player->GetPosition(),								// �v���C�����W
			obj.position,											// �u���b�N���W
			DirectX::SimpleMath::Vector3{ m_player->GetSize() },	// �v���C���T�C�Y
			DirectX::SimpleMath::Vector3{ COMMON_SIZE }				// �u���b�N�T�C�Y
		);

		// �������Ă����珈������
		if (is_boxCol.GetHitBoxFlag())
		{
			// �Փ˂����I�u�W�F�N�g�����X�g�ɒǉ�
			m_colObjList.push_back(obj);
		}
	}
}

//--------------------------------------------------------//
//�����߂�������                                          //
//--------------------------------------------------------//
void PlayScene::ApplyPushBack(Object& obj)
{
	// �����Ă���I�u�W�F�N�g����C�ȊO�̏ꍇ�͉����߂���L���ɂ���
	if (obj.id != MapLoad::BoxState::None)
	{
		is_boxCol.SetPushMode(true);
	}
	else
	{
		is_boxCol.SetPushMode(false);
	}

	// �����Ă���̂���C�̏ꍇ�͏������Ȃ�
	if (obj.id == MapLoad::BoxState::None) return;

	// �R�C���̏���
	if (obj.id == MapLoad::BoxState::CoinBox)
	{ 
		// �Y���R�C���̌������l������
		for (auto& i : m_mapObj)
		{
			if (i == obj)
			{
				test_count += 1;				// �l�������Z
				i.id = MapLoad::BoxState::None;	// �l��������R�C�����폜
			}
		}
		return;
	}

	// �v���C���̃|�W�V������ۑ�
	DirectX::SimpleMath::Vector3 playerPos = m_player->GetPosition();

	// �����蔻�������ĉ����߂�
	is_boxCol.PushBox(
		&playerPos,												// �v���C�����W
		obj.position,											// �u���b�N���W
		DirectX::SimpleMath::Vector3{ m_player->GetSize() },	// �v���C���T�C�Y
		DirectX::SimpleMath::Vector3{ COMMON_SIZE }				// �u���b�N�T�C�Y
	);

	// �ύX��̃v���C���̃|�W�V�����𔽉f
	m_player->SetPosition(playerPos);

	// �u���b�N�̏�ɓ������Ă�����d�͂�������
	if (is_boxCol.GetHitFace() == Collider::BoxCollider::HIT_FACE::UP)
	{
		m_player->ResetGravity();
	}

	// �������I�������v�f��j��
	m_colObjList.pop_back();
}

//--------------------------------------------------------//
//�f�o�b�O�\��                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto state = GetSystemManager()->GetCommonStates().get();

	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// �V�[�����̕\��
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,0 }, L"PlayScene");

	// �������ݒ�
	wchar_t cam[64];

	// �J�����̃|�W�V����
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().z)
	);
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,20 }, cam);

	// �X�e�[�W�ԍ��m�F
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,40 }, num);
	
	// �v���C���̍��W
	wchar_t plr[64];
	swprintf_s(plr, 64, L"PlayerPosition = %f,%f,%f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,60 }, plr);
	
	// �v���C���̏d��
	wchar_t gra[32];
	swprintf_s(gra, 32, L"Gravity = %f", m_player->GetGravity());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,80 }, gra);
		
	// �R�C���e�X�g
	wchar_t coi[32];
	swprintf_s(coi, 32, L"Coin = %d", test_count);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,100 }, coi);

	// �f�o�C�X�R���e�L�X�g�̎擾�F�O���b�h�̕`��Ɏg�p
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// �f�o�b�O�O���b�h�̕\��
	GetSystemManager()->GetGridFloor()->Draw(context, state, view, proj);
}

//--------------------------------------------------------//
//�}�b�v�ǂݍ���                                          //
//--------------------------------------------------------//
void PlayScene::LoadMap(int num)
{
	// �t�@�C�����̐錾
	std::wstring filePath = L"";

	// �}�b�v�̕ύX
	switch (num)
	{
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	// �}�b�v�̓ǂݍ���
	m_map.LoadMap(filePath);

	// �}�b�v�̊i�[
	m_mapObj = m_map.GetMapData();

	// �������̉��
	m_map.ReleaseMemory();
	
	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_mapObj[i].position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}