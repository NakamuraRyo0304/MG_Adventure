/*
 *	@File	ColliderHelper.cpp
 *	@Brief	�����蔻��̃w���p�[�N���X�B
 *	@Date	2023-01-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Game/PlayScene/Objects/Player.h"
#include "Game/CommonObjects/Blocks.h"
#include "ColliderHelper.h"

 // �G�C���A�X�錾
using BOXHIT = Collider::BoxCollider::HIT_FACE;

// �R���X�g���N�^
ColliderHelper::ColliderHelper(const float& radius, std::shared_ptr<SystemManager> system)
	: m_radius{ radius }
	, m_system{ system }
{
	// �����蔻��̍쐬
	m_collider = std::make_unique<Collider::BoxCollider>();

	// �ڐG�I�u�W�F�N�g�̏�����
	m_hitObj.clear();
}

// �f�X�g���N�^
ColliderHelper::~ColliderHelper()
{
	m_hitObj.clear();
	m_collider.reset();
}

// �X�V����
void ColliderHelper::Update(Player* player, Blocks* blocks)
{
	// �����蔻��̍X�V
	Judgement(player, blocks);

	// �Փ˂����I�u�W�F�N�g���Ƃɉ����߂��������s��
	for (auto& i : m_hitObj)
	{
		ApplyPushBack(player, blocks, i);
	}
}


// ���菈��
void ColliderHelper::Judgement(Player* player, Blocks* blocks)
{
	// �Փ˂����I�u�W�F�N�g���X�g��������
	m_hitObj.clear();

	// �����蔻������
	for (auto& block : blocks->GetMapData())
	{
		// �u���b�N���Ȃ���Ώ������Ȃ�
		if (block.id == MAPSTATE::NONE) continue;

		// �v���C���̔���͈͊O�͏������Ȃ�
		if (not UserUtility::CheckPointInSphere(
			player->GetPosition(), m_radius, block.position)) continue;

		// ��������
		m_collider->Judgement(
			player->GetPosition(), block.position,				// �v���C���A�I�u�W�F�N�g�̍��W
			SimpleMath::Vector3{ player->GetSize() },			// �v���C���T�C�Y
			SimpleMath::Vector3{ blocks->GetObjSize(block.id) }	// �u���b�N�T�C�Y
		);

		// �������Ă�����R���W�������X�g�ɒǉ�
		if (m_collider->IsHitBoxFlag())
		{
			m_hitObj.push_back(block);
		}
	}
}

// �����߂�����
void ColliderHelper::ApplyPushBack(Player* player, Blocks* blocks, Object& obj)
{
	// �����Ă���I�u�W�F���Ȃ��̏ꍇ�͏������Ȃ�
	if (obj.id == MAPSTATE::NONE)
	{
		// �v�f�������ďI��
		m_hitObj.pop_back();
		return;
	}

	// �f�t�H���g�Ŕ��������
	m_collider->SetPushMode(true);

	// �R�C���̊l������
	if (obj.id == MAPSTATE::COIN)
	{
		// �����߂����Ȃ�
		m_collider->SetPushMode(false);

		// �R�C���J�E���g�A�b�v
		blocks->CountUpCoin(obj.index);

		// �R�C���l������炷
		m_system->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_COINGETTER, false);
	}

	// �_�̕��㏈��
	if (obj.id == MAPSTATE::CLOUD)
	{
		// �v���C���[�����ɂ����牟���߂����Ȃ�
		if (player->GetPosition().y < obj.position.y + blocks->GetObjSize(MAPSTATE::CLOUD))
		{
			// �v�f�������ďI��
			m_hitObj.pop_back();
			return;
		}

		// �����L����
		m_collider->SetPushMode(true);

		// �C���f�b�N�X�ԍ����i�[
		m_lastIdx.push_back(obj.index);

		// ��Ȃ珈�����Ȃ�
		if (m_lastIdx.empty()) return;

		// �������Ă��锻����o��
		blocks->SetCloudHitFlag(m_lastIdx.front(), true);
		m_lastIdx.pop_front();
	}

	// �d�͏���
	if (obj.id == MAPSTATE::GRAVITY)
	{
		m_collider->SetPushMode(false);
		blocks->CallGravity();
	}

	// �v���C���[�̉����߂�
	SimpleMath::Vector3 _playerPos = player->GetPosition();
	m_collider->Judgement(
		&_playerPos, obj.position,
		SimpleMath::Vector3{ player->GetSize() },
		SimpleMath::Vector3{ blocks->GetObjSize(obj.id) });
	player->SetPosition(_playerPos);

	// �u���b�N�̏�ɏ���Ă����璅�n����
	if (m_collider->GetHitFace() == BOXHIT::UP)
	{
		player->ResetGravity();
	}

	// �v�f�������ďI��
	m_hitObj.pop_back();
}