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
			static const unsigned int RIGHT = 1;		// �E
			static const unsigned int LEFT  = 2;		// ��
			static const unsigned int UP    = 3;		// ��
			static const unsigned int DOWN  = 4;		// ��
			static const unsigned int FRONT = 5;		// �O
			static const unsigned int BACK  = 6;		// ���
			static const unsigned int NONE  = 9;		// �����Ă��Ȃ�
		};

	public:
		BoxCollider();
		~BoxCollider() = default;

		/// <summary>
		/// �����߂�����
		/// </summary>
		/// <param name= moveObj">�����߂������I�u�W�F</param>
		/// <param name= constObj">�Ԃ���ΏۃI�u�W�F</param>
		/// <param name= sz1">�傫��</param>
		/// <param name= sz2">�傫��</param>
		/// <returns>�Ȃ�</returns>
		void PushBox(DirectX::SimpleMath::Vector3* moveObj,
		       const DirectX::SimpleMath::Vector3& constObj,
		       const DirectX::SimpleMath::Vector3& sz1,
		       const DirectX::SimpleMath::Vector3& sz2);

		/// <summary>
		/// �������Ă��邩�ǂ����̂ݔ��肷��(�ʂ��i�[�����)
		/// </summary>
		/// <param name= moveObj">�����߂������I�u�W�F(�����Ă������)</param>
		/// <param name= constObj">�Ԃ���ΏۃI�u�W�F(��苭������)</param>
		/// <param name= sz1">�傫��</param>
		/// <param name= sz2">�傫��</param>
		/// <returns>�Ȃ�</returns>
		void PushBox(const DirectX::SimpleMath::Vector3& moveObj,
					 const DirectX::SimpleMath::Vector3& constObj,
					 const DirectX::SimpleMath::Vector3& sz1,
					 const DirectX::SimpleMath::Vector3& sz2);

		/// <summary>
		/// �ʂ̔���
		/// </summary>
		/// <param name="��������"></param>
		/// <returns>�������Ă���ʂ�Ԃ�</returns>
		const unsigned int& GetHitFace() { return m_hitFace; }

		/// <summary>
		/// �������Ă��邩����
		/// </summary>
		/// <param name="��������"></param>
		/// <returns>�������Ă�����True��Ԃ�</returns>
		const bool& IsHitBoxFlag() { return is_hitFlag; }

		/// <summary>
		/// �����߂���L���ɂ���֐�
		/// </summary>
		/// <param name="flag">�L���ɂ���ꍇTrue�ɂ���</param>
		/// <returns>�Ȃ�</returns>
		void SetPushMode(const bool& flag) { is_pushMode = flag; }
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

		/// <summary>
		/// ���̓����蔻��
		/// </summary>
		/// <param name="pos1">���W</param>
		/// <param name="pos2">���W</param>
		/// <param name="radius1">���a</param>
		/// <param name="radius2">���a</param>
		/// <returns>�Ȃ�</returns>
		void PushSphere(DirectX::SimpleMath::Vector3& pos1,
			            DirectX::SimpleMath::Vector3& pos2,
			            float radius1,float radius2);

		/// <summary>
		/// �������Ă��邩���肷��
		/// </summary>
		/// <param name="��������"></param>
		/// <returns>�������Ă�����True��Ԃ�</returns>
		const bool& IsHitSphereFlag() { return is_hitFlag; }

		/// <summary>
		/// �����߂���L���ɂ���֐�
		/// </summary>
		/// <param name="flag">True�ŉ����߂���L���ɂ���</param>
		/// <returns>�Ȃ�</returns>
		void SetPushMode(const bool& flag) { is_pushMode = flag; }

	};

	// AABB�R���C�_�[
	class AABBCollider
	{
	private:
	public:
		AABBCollider();
		~AABBCollider() = default;

		/// <summary>
		/// �񎟌��l�p�`�̔���
		/// </summary>
		/// <param name="pos1">���W�P</param>
		/// <param name="pos2">���W�Q</param>
		/// <param name="sz1">�T�C�Y�P</param>
		/// <param name="sz2">�T�C�Y�Q</param>
		/// <returns>�������Ă�����True��Ԃ�</returns>
		bool IsHitAABB2D(const DirectX::SimpleMath::Vector2& pos1,
						 const DirectX::SimpleMath::Vector2& pos2,
						 const DirectX::SimpleMath::Vector2& sz1,
						 const DirectX::SimpleMath::Vector2& sz2);
	};
}

#endif // COLLIDER