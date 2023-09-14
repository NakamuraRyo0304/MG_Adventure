/*
 *	@File	Collider.cpp
 *	@Brief	�����m�̓����蔻��B
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Collider.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Collider::BoxCollider::BoxCollider():
	m_hitFace(),
	is_hitFlag(),
	is_pushMode()
{
	m_hitFace = HIT_FACE::NONE;
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// �����߂�����
/// </summary>
/// <param name= moveObj">�����߂������I�u�W�F</param>
/// <param name= constObj">�Ԃ���ΏۃI�u�W�F</param>
/// <param name= sz1">�傫��</param>
/// <param name= sz2">�傫��</param>
/// <returns>�Ȃ�</returns>
void Collider::BoxCollider::PushBox(SimpleMath::Vector3* moveObj,
			                        const SimpleMath::Vector3& constObj,
			                        const SimpleMath::Vector3& sz1,
	                                const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����Ă���ʒu��������
	m_hitFace = HIT_FACE::NONE;

	// ���S�ʒu���v�Z
	SimpleMath::Vector3 cS1 = sz1 / 2, cS2 = sz2 / 2;

	// �����蔻������
	if (!(moveObj->x - cS1.x < constObj.x + cS2.x &&
		  moveObj->x + cS1.x > constObj.x - cS2.x &&
		  moveObj->z - cS1.z < constObj.z + cS2.z &&
		  moveObj->z + cS1.z > constObj.z - cS2.z &&
		  moveObj->y - cS1.y < constObj.y + cS2.y &&
		  moveObj->y + cS1.y > constObj.y - cS2.y)) return;

	// �����Ă���
	is_hitFlag = true;

	// ���g�̕��ƍ������v�Z
	float lengthX = (moveObj->x + cS1.x) - (moveObj->x - cS1.x);
	float lengthY = (moveObj->y + cS1.y) - (moveObj->y - cS1.y);
	float lengthZ = (moveObj->z + cS1.z) - (moveObj->z - cS1.z);

	// �e�����̂߂荞�݋
	float leftRatio  = ((moveObj->x + cS1.x) - (constObj.x - cS2.x)) / lengthX;
	float rightRatio = ((constObj.x + cS2.x) - (moveObj->x - cS1.x)) / lengthX;

	float upRatio    = ((moveObj->y + cS1.y) - (constObj.y - cS2.y)) / lengthY;
	float downRatio  = ((constObj.y + cS2.y) - (moveObj->y - cS1.y)) / lengthY;

	float frontRatio = ((moveObj->z + cS1.z) - (constObj.z - cS2.z)) / lengthZ;
	float backRatio  = ((constObj.z + cS2.z) - (moveObj->z - cS1.z)) / lengthZ;

	// �ő�l���Z�o
	float maxRatio = std::max({ leftRatio ,rightRatio ,upRatio ,downRatio ,frontRatio ,backRatio });

	// �����Ă���ʒu���i�[����
	m_hitFace = maxRatio == leftRatio  ? HIT_FACE::LEFT  : m_hitFace; // �������ʒu�F�� or �ω��Ȃ�
	m_hitFace = maxRatio == rightRatio ? HIT_FACE::RIGHT : m_hitFace; // �������ʒu�F�E or �ω��Ȃ�
	m_hitFace = maxRatio == upRatio    ? HIT_FACE::UP    : m_hitFace; // �������ʒu�F�� or �ω��Ȃ�
	m_hitFace = maxRatio == downRatio  ? HIT_FACE::DOWN  : m_hitFace; // �������ʒu�F�� or �ω��Ȃ�
	m_hitFace = maxRatio == frontRatio ? HIT_FACE::FRONT : m_hitFace; // �������ʒu�F�O or �ω��Ȃ�
	m_hitFace = maxRatio == backRatio  ? HIT_FACE::BACK  : m_hitFace; // �������ʒu�F�� or �ω��Ȃ�

	// �����߂����������Ȃ��ꍇ���^�[��
	if (!is_pushMode) return;

	// �����Ă���ʒu���i�[���A�����߂��������s��
	(*moveObj).x = maxRatio == leftRatio  ? constObj.x + (sz1.x + sz2.x) / 2 : (*moveObj).x; // �������ʒu�F�� or �ω��Ȃ�
	(*moveObj).x = maxRatio == rightRatio ? constObj.x - (sz1.x + sz2.x) / 2 : (*moveObj).x; // �������ʒu�F�E or �ω��Ȃ�
	(*moveObj).y = maxRatio == upRatio    ? constObj.y + (sz1.y + sz2.y) / 2 : (*moveObj).y; // �������ʒu�F�� or �ω��Ȃ�
	(*moveObj).y = maxRatio == downRatio  ? constObj.y - (sz1.y + sz2.y) / 2 : (*moveObj).y; // �������ʒu�F�� or �ω��Ȃ�
	(*moveObj).z = maxRatio == frontRatio ? constObj.z + (sz1.z + sz2.z) / 2 : (*moveObj).z; // �������ʒu�F�O or �ω��Ȃ�
	(*moveObj).z = maxRatio == backRatio  ? constObj.z - (sz1.z + sz2.z) / 2 : (*moveObj).z; // �������ʒu�F�� or �ω��Ȃ�
}

/// <summary>
/// �������Ă��邩�ǂ����̂ݔ��肷��(�ʂ��i�[�����)
/// </summary>
/// <param name= moveObj">�����߂������I�u�W�F(�����Ă������)</param>
/// <param name= constObj">�Ԃ���ΏۃI�u�W�F(��苭������)</param>
/// <param name= sz1">�傫��</param>
/// <param name= sz2">�傫��</param>
/// <returns>�Ȃ�</returns>
void Collider::BoxCollider::PushBox(const SimpleMath::Vector3& moveObj,
									const SimpleMath::Vector3& constObj,
									const SimpleMath::Vector3& sz1,
									const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����蔻������
	if (!(moveObj.x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		  moveObj.x + sz1.x / 2 > constObj.x - sz2.x / 2 &&
		  moveObj.z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		  moveObj.z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		  moveObj.y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		  moveObj.y + sz1.y / 2 > constObj.y - sz2.y / 2)) return;

	// �����Ă���
	is_hitFlag = true;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// ���̓����蔻��
/// </summary>
/// <param name="pos1">���W</param>
/// <param name="pos2">���W</param>
/// <param name="radius1">���a</param>
/// <param name="radius2">���a</param>
/// <returns>�Ȃ�</returns>
void Collider::SphereCollider::PushSphere(SimpleMath::Vector3& pos1,
	                                      SimpleMath::Vector3& pos2,
	                                      float radius1, float radius2)
{
	// ��̋��̋������v�Z
	float distance = SimpleMath::Vector3::Distance(pos1, pos2);

	// ��̋��̔��a�̘a���v�Z
	float sumOfRadius = radius1 + radius2;

	// ��̋��̔��a�̘a������_�Ԃ̋������Z�������瓖���Ă��锻��ɂ���
	if (distance <= sumOfRadius)
	{
		// �Փ˂������̕����x�N�g�����v�Z
		SimpleMath::Vector3 collisionDirection = pos2 - pos1;

		// ���K������
		collisionDirection.Normalize();

		// �Փ˂������̉����߂��������v�Z
		float pushDistance = sumOfRadius - distance;

		if (is_pushMode)
		{
			// ��1�������߂�
			pos1 -= collisionDirection * pushDistance / 2;

			// ��2�������߂�
			pos2 += collisionDirection * pushDistance / 2;
		}

		is_hitFlag = true;
	}

	is_hitFlag = false;
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Collider::AABBCollider::AABBCollider()
{
}

/// <summary>
/// ���ʂ�AABB�����蔻��
/// </summary>
/// <param name="pos1">���W</param>
/// <param name="pos2">���W</param>
/// <param name="sz1">�傫��</param>
/// <param name="sz2">�傫��</param>
/// <returns>�������Ă�����True</returns>
// ��P�A�Q�����F���W ��R�A�S�����F�T�C�Y
bool Collider::AABBCollider::IsHitAABB2D(const SimpleMath::Vector2& pos1,
										 const SimpleMath::Vector2& pos2,
										 const SimpleMath::Vector2& sz1,
										 const SimpleMath::Vector2& sz2)
{
	// XZ���ʂɂ����铖���蔻��
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.y - sz1.y / 2 < pos2.y + sz2.y / 2 &&
		pos1.y + sz1.y / 2 > pos2.y - sz2.y / 2)return true;

	return false;
}

