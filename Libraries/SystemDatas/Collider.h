/*
 *	@File	Collider.h
 *	@Brief	�����蔻��N���X�B
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef COLLIDER
#define COLLIDER

#include "SimpleMath.h"

namespace Collider
{
	// �{�b�N�X�R���C�_�[�N���X
	class BoxCollider
	{
	private:
		// �����Ă���ʂ𔻒�
		unsigned int m_hitFace;
		// �����Ă��邩����
		bool is_hitFlag;
		// �����߂��������s�������߂�t���O
		bool is_pushMode;

	public:
		// �������ʒu
		struct HIT_FACE
		{
			static const unsigned int RIGHT = 11;		// �E
			static const unsigned int LEFT  = 22;		// ��
			static const unsigned int UP    = 33;		// ��
			static const unsigned int DOWN  = 44;		// ��
			static const unsigned int FRONT = 55;		// �O
			static const unsigned int BACK  = 66;		// ���
			static const unsigned int NONE  = 99;		// �����Ă��Ȃ�
		};

	public:
		BoxCollider();
		~BoxCollider() = default;

		// �����߂����� �����F&���������,�����Ȃ����,�傫���P,�傫���Q
		void PushBox(DirectX::SimpleMath::Vector3* moveObj,
		       const DirectX::SimpleMath::Vector3& constObj,
		       const DirectX::SimpleMath::Vector3& sz1,
		       const DirectX::SimpleMath::Vector3& sz2);
		// �����蔻��̂ݎ��
		void PushBox(DirectX::SimpleMath::Vector3& moveObj,
		       const DirectX::SimpleMath::Vector3& constObj,
		       const DirectX::SimpleMath::Vector3& sz1,
		       const DirectX::SimpleMath::Vector3& sz2);

		// �������ʒu��Ԃ�
		unsigned int GetHitFace() { return m_hitFace; }

		// ������������
		const bool& GetHitBoxFlag() { return is_hitFlag; }

		// �����߂����������邩
		void SetPushMode(bool flag) { is_pushMode = flag; }
	};

	// �X�t�B�A�R���C�_�[�N���X
	class SphereCollider
	{
	private:
		// �����Ă��邩����
		bool is_hitFlag;
		// �����߂��������s�������߂�t���O
		bool is_pushMode;

	public:
		SphereCollider();
		~SphereCollider() = default;

		// �����蔻��Ɖ����߂�
		void PushSphere(DirectX::SimpleMath::Vector3& pos1,
			            DirectX::SimpleMath::Vector3& pos2,
			            float radius1,float radius2);

		// �����Ă��邩�ǂ������肷��
		const bool& GetHitSphereFlag() { return is_hitFlag; }

		// �����߂����������邩
		void SetPushMode(const bool& flag) { is_pushMode = flag; }

	};

	// AABB�R���C�_�[
	class AABBCollider
	{
	private:
	public:
		AABBCollider();
		~AABBCollider() = default;

		// �����Ă��邩�ǂ�����Ԃ�
		bool HitAABB_2D(const DirectX::SimpleMath::Vector2& pos1,
			const DirectX::SimpleMath::Vector2& pos2,
			const DirectX::SimpleMath::Vector2& sz1,
			const DirectX::SimpleMath::Vector2& sz2);
	};
}

#endif // COLLIDER