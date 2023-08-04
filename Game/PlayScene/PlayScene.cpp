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

// �V�F�[�_�[
#include "Objects/PlayerBill.h"
#include "Objects/PlayerShadow.h"

// �v���C���N���X
#include "Objects/Player.h"

// �u���b�N�N���X
#include "Objects/Blocks.h"

// �C���^�[�t�F�[�X
#include "Objects/PlayUI.h"

#include "PlayScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
PlayScene::PlayScene() :
	IScene(),
	m_timer{0.0f},					// �^�C�}�[
	m_startTimer{0.0f},				// �J�n����
	m_timeLimit{0.0f},				// ��������
	m_returnTimeVal{0.0f},			// ��������(�߂�l)
	m_mapLoad{},					// �}�b�v
	m_stageNum{1},					// �X�e�[�W�ԍ�
	m_fallValue{0.0f},				// �����p�ϐ�
	m_prevIndex{},					// �ߋ��ɓ��������C���f�b�N�X�ԍ�
	m_hitObjects{},					// �����Ă���I�u�W�F�N�g�̊i�[
	m_lastObj{},					// �Ō�ɓ��������I�u�W�F�N�g��ۑ�
	is_boxCol{},					// �����̓����蔻��
	m_skyDomeModel{ nullptr },		// �X�J�C�h�[�����f��
	m_skyColor{}					// ��̕ω�
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayScene::~PlayScene()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(true);

	// �v���C���̏�����
	m_player->Initialize(std::make_shared<SystemManager>());

	// �}�b�v�ǂݍ���
	m_blocks->Initialize(m_stageNum);

	// �v���C���[���W�ݒ�
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// ����̏�����
	m_hitObjects.clear();

	// �������Ԃ̏�����
	// ���ԁ@�~�@�t���[�����[�g
	m_timeLimit = TIME_LIMIT * FLAME_RATE;

	// �J�n�J�E���g�_�E��(�t�F�[�h���l��)
	m_startTimer = 4 * FLAME_RATE;

	// ���S�G�t�F�N�g��؂�
	m_userInterFace->SetEffectFlag(false);

	// ��̐F�̏�����
	m_skyColor = { 1.0f,1.0f,1.0f };
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">����/fps</param>
/// <param name="keyState">�L�[�{�[�h�|�C���^</param>
/// <param name="mouseState">�}�E�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// �L�[���͏����擾����
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// �}�E�X�����擾����
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �J�E���g�_�E�����I�������X�^�[�g
	if (StartTimer() == false) return;

	// �R�C�������ׂĊl���Ń��U���g
	if (m_blocks->IsCollectedFlag() || m_timeLimit < 0.0f)
	{
		ChangeScene(SCENE::RESULT);
		return;
	}
	else // �N���A���Ă��Ȃ���΃f�N�������g
	{
		// �������Ԃ̌v�Z
		m_timeLimit--;

		// �N���A�^�C�����i�[
		m_returnTimeVal = m_timeLimit / FLAME_RATE;

		// ��̏���
		UpdateSky();
	}

	// �v���C���̍X�V
	m_player->Update(keyState, elapsedTime);

	// ���Έړ�
	m_playerBill->SetVertexMovePos(m_player->GetPosition());

	// �X�e�[�W�̉��ɗ�������X�e�[�W���󉉏o
	if (m_player->GetPosition().y < DURATION_FLOOR_LINE)
	{
		// ����������
		m_fallValue -= FALL_SPEED;

		for (auto& obj : m_blocks->GetMapData())
		{
			// �G�t�F�N�g���I�t
			m_userInterFace->SetEffectFlag(false);

			// �I�u�W�F�N�g�̐U��
			GetSystemManager()->GetCamera()->ShakeObject(
				SHAKE_DURATION,							// �U������
				SHAKE_TREMOR * 2.5f,					// �U���͈�
				&m_blocks->GetBlockPosition(obj.index)	// �U���I�u�W�F�N�g
			);

			// ���W�̃Z�b�g
			m_blocks->SetBlockPosition(
				SimpleMath::Vector3(
				obj.position.x,							// X��
				obj.position.y + m_fallValue,			// Y��
				obj.position.z),						// Z��
				obj.index								// �z��ԍ�
			);
		}
	}
	// ���������玀�S�G�t�F�N�g���o��
	else if (m_player->GetPosition().y < DURATION_FLOOR_LINE / 1.5f)
	{
		// �I�u�W�F�N�g�̐U��
		for (auto& obj : m_blocks->GetMapData())
		{
			// �G�t�F�N�g���I��
			if (static_cast<int>(m_player->GetPosition().y) % 2 == 0)
			{
				m_userInterFace->SetEffectFlag(true);
			}
			else
			{
				m_userInterFace->SetEffectFlag(false);
			}
			GetSystemManager()->GetCamera()->ShakeObject(
				1.0f,									// �U������
				SHAKE_TREMOR,							// �U���͈�
				&m_blocks->GetBlockPosition(obj.index)	// �U���I�u�W�F�N�g
			);
		}
	}
	// �v���C���[�������̂ݏ���
	else
	{
		// �u���b�N�̍X�V
		m_blocks->Update(elapsedTime);

		// �v���C���[�̉e�V�F�[�_�[�̍X�V
		m_playerShadow->Update(
			SimpleMath::Vector3(
				m_player->GetPosition().x,
				m_lastObj.position.y + 0.55f,
				m_player->GetPosition().z
			)
		);

		// �����蔻��̍X�V
		Judgement();
	}

	// UI�̍X�V
	m_userInterFace->Update(m_timeLimit);

	// ���������烊�X�^�[�g
	if (m_player->GetDeathFlag())
	{
		ChangeScene(SCENE::PLAY);
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayScene::Draw()
{
	// �`��֘A
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// �J�����p�s��
	SimpleMath::Matrix view, proj;

	// �r���[�s��
	view = GetSystemManager()->GetCamera()->GetView();

	// �v���W�F�N�V�����s��
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// �v���C���̕`��
	m_player->Render(context, states, view, proj);

	// �}�b�v�̕`��
	m_blocks->Render(context, states, view, proj, m_timer);

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);
	m_skyDomeModel->UpdateEffects([&](IEffect* effect)
		{
			// �F�����X�ɈÂ�����
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// ���X�ɈÂ��Ȃ��Ă���
				basicEffect->SetEmissiveColor(
					SimpleMath::Color{
						m_skyColor.red,		 // ��
						m_skyColor.green,	 // ��
						m_skyColor.blue,	 // ��
						1.0f				 // �����x
					}
				);
			}
		}
	);

	// �r���{�[�h�쐬
	m_playerBill->CreateBillboard(
		GetSystemManager()->GetCamera()->GetTargetPosition(),	// �J�����̒����_
		GetSystemManager()->GetCamera()->GetEye(),				// �J�����̍��W
		SimpleMath::Vector3::Up
	);
	// �r���{�[�h�̕`��
	m_playerBill->Render(m_player->GetPosition(), m_timer, view, proj);

	// �v���C���[�̉e�̕`��
	m_playerShadow->Render(context, view, proj);

	// UI�̕`��
	if (static_cast<int>(m_startTimer) == 0)
	{
		m_userInterFace->Render();
	}
	else
	{
		m_userInterFace->RenderCountDown(m_startTimer);
	}
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayScene::Finalize()
{
	// �v���C���̌㏈��
	m_player->Finalize();

	// �}�b�v�̌㏈��
	m_blocks->Finalize();

	// ����p�z������
	m_hitObjects.clear();
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
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

	//-------------------------------------------------------------------------------------//

	// �X�J�C�h�[�����f�����쐬����
	m_skyDomeModel = ModelFactory::GetCreateModel(
		device,
		L"Resources/Models/ShineSky.cmo"
	);
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

	//-------------------------------------------------------------------------------------//

	// �v���C���[�̍쐬
	m_player = std::make_unique<Player>(
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/Head.cmo")),	// ��
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/Body.cmo")),	// �{�f�B
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/LegR.cmo")),	// �E��
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/LegL.cmo"))	// ����
	);

	//-------------------------------------------------------------------------------------//

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

	//-------------------------------------------------------------------------------------//

	// �ʒu���̃V�F�[�_�[�̍쐬
	m_playerBill = std::make_unique<PlayerBill>();
	m_playerBill->Create(GetSystemManager()->GetDeviceResources());

	// �v���C���[�̉e�V�F�[�_�[�̍쐬
	m_playerShadow = std::make_unique<PlayerShadow>();
	m_playerShadow->Create(GetSystemManager()->GetDeviceResources());

	//-------------------------------------------------------------------------------------//

	// UI�̍쐬
	m_userInterFace = std::make_unique<PlayUI>(SimpleMath::Vector2(width, height));
	m_userInterFace->Create(GetSystemManager(),context, device);
}

/// <summary>
/// �����蔻�菈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayScene::Judgement()
{
	// �Փ˂����I�u�W�F�N�g���X�g��������
	m_hitObjects.clear();

	// �����蔻������
	for (auto& obj : m_blocks->GetMapData())
	{
		// �v���C���̔��a2.0f�͈̔͂ɂȂ���Ώ������Ȃ�
		if (UserUtility::CheckPointInSphere(
			m_player->GetPosition(),									// ���S�_
			JUDGE_AREA,													// ���a
			obj.position))												// ���邩���ׂ�_
		{
			// ��������
			is_boxCol.PushBox(
				m_player->GetPosition(),								// �v���C�����W
				obj.position,											// �R�C�����W
				SimpleMath::Vector3{ m_player->GetSize() },				// �v���C���T�C�Y
				SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id) }		// �u���b�N�T�C�Y
			);

			// �������Ă����珈������
			if (is_boxCol.GetHitBoxFlag())
			{
				// �Փ˂����I�u�W�F�N�g�����X�g�ɒǉ�
				m_hitObjects.push_back(obj);
			}
		}
	}

	// �Փ˂����I�u�W�F�N�g���Ƃɉ����߂��������s��
	for (auto& obj : m_hitObjects)
	{
		ApplyPushBack(obj);
	}
}

/// <summary>
/// �����߂�����
/// </summary>
/// <param name="obj">���������I�u�W�F�N�g</param>
/// <returns>�Ȃ�</returns>
void PlayScene::ApplyPushBack(Object& obj)
{
	// �����Ă���I�u�W�F����C�̏ꍇ�͏������Ȃ�
	if (obj.id == MapState::None)
	{
		is_boxCol.SetPushMode(false);
		return;
	}
	else
	{
		is_boxCol.SetPushMode(true);
	}

	//-------------------------------------------------------------------------------------//

	// �R�C���̏���
	if (obj.id == MapState::CoinBox)
	{
		// �����߂����Ȃ�
		is_boxCol.SetPushMode(false);

		// �R�C���J�E���g�A�b�v
		m_blocks->CountUpCoin(obj.index);
	}

	//-------------------------------------------------------------------------------------//

	// �_�̏���
	if (obj.id == MapState::ClowdBox)
	{
		// �v���C���[�����ɂ����牟���߂����Ȃ�
		if (m_player->GetPosition().y < obj.position.y + m_blocks->GetObjSize(obj.id))
		{
			is_boxCol.SetPushMode(false);
			return;
		}

		// �����L����
		is_boxCol.SetPushMode(true);

		// �C���f�b�N�X�ԍ����i�[
		m_prevIndex.push_back(obj.index);

		// �������Ă��锻����o��
		m_blocks->SetClowdHitFlag(m_prevIndex.front(), true);

		// ��Ȃ珈�����Ȃ�
		if (m_prevIndex.empty()) return;

		// �����Ă�����擪���폜
		m_prevIndex.pop_front();
	}


	// ���Z�b�g����
	if (obj.id == MapState::ResetClowd)
	{
		is_boxCol.SetPushMode(false);
		m_blocks->RestoreClowdPosition();
	}

	//-------------------------------------------------------------------------------------//

	// �v���C���̃|�W�V������ۑ�
	SimpleMath::Vector3 playerPos = m_player->GetPosition();

	// �����蔻�������ĉ����߂�
	is_boxCol.PushBox(
		&playerPos,												// �v���C�����W
		obj.position,											// �u���b�N���W
		SimpleMath::Vector3{ m_player->GetSize() },				// �v���C���T�C�Y
		SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id)}		// �u���b�N�T�C�Y
	);

	// �ύX��̃v���C���̃|�W�V�����𔽉f
	m_player->SetPosition(playerPos);

	// �u���b�N�̏�ɓ������Ă�����d�͂�������
	if (is_boxCol.GetHitFace() == Collider::BoxCollider::HIT_FACE::UP)
	{
		m_player->ResetGravity();
	}

	//-------------------------------------------------------------------------------------//

	// �������I�������v�f��j��
	m_hitObjects.pop_back();
}

/// <summary>
/// ��̍X�V
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayScene::UpdateSky()
{
	m_skyColor =
	{
		m_timeLimit / (TIME_LIMIT * FLAME_RATE), // ��
		m_timeLimit / (TIME_LIMIT * FLAME_RATE), // ��
		m_timeLimit / (TIME_LIMIT * FLAME_RATE)  // ��
	};
}

/// <summary>
/// �X�^�[�g�̃J�E���g�_�E��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
bool PlayScene::StartTimer()
{
	m_startTimer--;

	if (m_startTimer <= 0.0f)
	{
		m_startTimer = 0.0f;
		return true;
	}

	return false;
}

/// <summary>
/// �f�o�b�O���O
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void PlayScene::DebugLog(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto state = GetSystemManager()->GetCommonStates().get();

	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);

	// �V�[�����̕\��
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,0 }, L"PlayScene");

	// �v���C���̍��W
	wchar_t plr[64];
	swprintf_s(plr, 64, L"PlayerPosition = %f,%f,%f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,20 }, plr);

	// �v���C���̏d��
	wchar_t gra[32];
	swprintf_s(gra, 32, L"Gravity = %f", m_player->GetGravity());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,40 }, gra);

	// �R�C���J�E���^
	wchar_t coi[32];
	swprintf_s(coi, 32, L"Coin = %d", m_blocks->GetCoinCount());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,60 }, coi);

	// �^�C�����~�b�g
	wchar_t time[32];
	swprintf_s(time, 32, L"Limit = %d", static_cast<int>(m_timeLimit));

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,80 }, time);

	// �f�o�C�X�R���e�L�X�g�̎擾�F�O���b�h�̕`��Ɏg�p
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// �f�o�b�O�O���b�h�̕\��
	GetSystemManager()->GetGridFloor()->Draw(context, state, view, proj);
}

/// <summary>
/// �R�C���Q�b�^�[
/// </summary>
/// <param name="��������"></param>
/// <returns>�l�������R�C���̖���</returns>
const int& PlayScene::GetCoinNum()
{
	return m_blocks->GetCoinCount();
}
