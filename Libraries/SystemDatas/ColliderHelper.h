/*
 *	@File	ColliderHelper.h
 *	@Brief	�����蔻��̃w���p�[�N���X�B
 *	@Date	2023-01-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef COLLIDERHELPER
#define COLLIDERHELPER

// �R���C�_�[
#include "Libraries/SystemDatas/Collider.h"
#include "Game/CommonObjects/Object.h"

class Player;
class Blocks;
class SystemManager;
class ColliderHelper
{
private:

	// ������s�����a
	float m_radius;

	// �I�u�W�F�N�g�̔���
	std::vector<Object> m_hitObj;

	// �O��̃C���f�b�N�X
	std::deque<UINT> m_lastIdx;

	// ����
	std::unique_ptr<Collider::BoxCollider> m_collider;

	// �R�C���̃T�E���h�p
	std::shared_ptr<SystemManager> m_system;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="radius">����̔��a</param>
	/// <param name="system">�V�X�e���}�l�[�W���|�C���^</param>
	/// <returns>�Ȃ�</returns>
	ColliderHelper(const float& radius, std::shared_ptr<SystemManager> system);
	~ColliderHelper();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="player">�v���C���|�C���^</param>
	/// <param name="blocks">�u���b�N�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void Update(Player* player, Blocks* blocks);


private:

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="player">�v���C���|�C���^</param>
	/// <param name="blocks">�u���b�N�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void Judgement(Player* player, Blocks* blocks);

	/// <summary>
	/// �����߂�����
	/// </summary>
	/// <param name="player">�v���C���|�C���^</param>
	/// <param name="blocks">�u���b�N�|�C���^</param>
	/// <param name="obj">���������I�u�W�F�N�g</param>
	/// <returns>�Ȃ�</returns>
	void ApplyPushBack(Player* player, Blocks* blocks, Object& obj);

};

#endif // COLLIDERHELPER
